/*
 * This file is part of parameters.
 *
 * Developed for the LSST Data Management System.
 * This product includes software developed by the LSST Project
 * (https://www.lsst.org).
 * See the COPYRIGHT file at the top-level directory of this distribution
 * for details of code ownership.
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
 */

#ifndef PARAMETERS_PARAMETER_H
#define PARAMETERS_PARAMETER_H

#include <cmath>
#include <iostream>
#include <limits>
#include <memory>
#include <string>

#ifndef PARAMETERS_LIMITS_H
#include "limits.h"
#endif

#ifndef PARAMETERS_TRANSFORM_H
#include "transform.h"
#endif

#ifndef PARAMETERS_TYPE_NAME_H
#include "type_name.h"
#endif

namespace parameters {

class Flag {
private:
    static inline const std::string _name = "";
    bool _value;

public:
    static const std::string get_name() { return _name; }
    bool get_value() const { return _value; }

    void set_value(bool value) { _value = value; }

    Flag(bool value) : _value(value) {}
};

template<typename T, class C>
class Parameter : public std::enable_shared_from_this<C> {
private:
    // TODO: Remember why this class needs to exist, if it does
    struct Limiter {
        const Limits<T> & limits;
        Limiter(const Limits<T> & limits) : limits(limits) {};
    };
    class Transformer {
    public:
        const Transform<T> & transform;

        virtual T get_value_transformed() const = 0;
        virtual T get_value_untransformed() const = 0;
        virtual void set_value_transformed(double value_transformed) = 0;
        virtual void set_value_untransformed(double value_untransformed) = 0;

        explicit Transformer(const Transform<T> & transform) : transform(transform) {};
        virtual ~Transformer() = default;
    };
    class TransformerNonNull : public Transformer {
    private:
        T * _value_transformed;

    public:
        T get_value_transformed() const { return *_value_transformed; }
        T get_value_untransformed() const { return this->transform.reverse(*_value_transformed); }
        void set_value_transformed(double value_transformed) { *_value_transformed = value_transformed; }
        void set_value_untransformed(double value_untransformed)
        {
            *_value_transformed = this->transform.forward(value_untransformed);
        }
        TransformerNonNull(const Transform<T> & transform) : Transformer(transform)
        {
            _value_transformed = new T;
        }
        ~TransformerNonNull() { delete _value_transformed; }
    };
    class TransformerNone : public Transformer {
    private:
        const double & _value;
    public:
        T get_value_transformed() const { return _value; };
        T get_value_untransformed() const { return _value; };
        void set_value_transformed(double) {};
        void set_value_untransformed(double) {};
        TransformerNone(const Transform<T> & transform, const T & value) : Transformer(transform),
            _value(value) {};
        ~TransformerNone() {}
    };

    static inline constexpr T _default = 0;
    static inline constexpr T _min = -std::numeric_limits<T>::infinity();
    static inline constexpr T _max = std::numeric_limits<T>::infinity();

    static inline const std::string _desc = "";
    static inline const std::string _name = "";

    template<typename P> static constexpr T _default_t(decltype(P::_default) *) { return P::_default; }
    template<typename P> static constexpr T _default_t(...) { return _default; }
    template<typename P> static constexpr T _min_t(decltype(P::_min) *) { return P::_min; }
    template<typename P> static constexpr T _min_t(...) { return _min; }
    template<typename P> static constexpr T _max_t(decltype(P::_max) *) { return P::_max; }
    template<typename P> static constexpr T _max_t(...) { return _max; }
    template<typename P> static const std::string _desc_t(decltype(P::_desc) *) { return P::_desc; }
    template<typename P> static const std::string _desc_t(...) { return _desc; }
    template<typename P> static const std::string _name_t(decltype(P::_name) *) { return P::_name; }
    template<typename P> static const std::string _name_t(...) { return _name; }

    std::unique_ptr<Limiter> _limiter;
    std::unique_ptr<Transformer> _transformer;

protected:
    T _value;
    std::shared_ptr<const Limits<T>> _limits_ptr;
    std::shared_ptr<const Transform<T>> _transform_ptr;

public:
    inline std::shared_ptr<Limits<T>> & _get_limits() { return _limits_ptr; }
    inline std::shared_ptr<Transform<T>> & _get_transform() { return _transform_ptr; }
    static const std::string get_desc() { return _desc_t<C>(0); }
    static constexpr T get_default() { return _default_t<C>(0); }
    static constexpr T get_min() { return _min_t<C>(0); }
    static constexpr T get_max() { return _max_t<C>(0); }
    static inline const std::string get_name() { return _name_t<C>(0); }
    inline const Limits<T> & get_limits() const { return _limiter->limits; }
    inline const Transform<T> & get_transform() const { return _transformer->transform; }
    static inline const std::string get_type_name() { return std::string(type_name<C>()); }
    T get_value() const { return _value; }
    T get_value_transformed() const { return _transformer->get_value_transformed(); }

    std::shared_ptr<C> ptr() { return this->shared_from_this(); }

    void set_limits(std::shared_ptr<const Limits<T>> limits) {
        if (limits == nullptr)
        {
            _limiter = std::make_unique<Limiter>(limits_maximal);
        } else {
            if (!((limits->get_min() >= this->get_min()) && (limits->get_max() <= this->get_max()))) {
                std::string error =
                    this->str() + ".set_limits(" + limits->str() + ") sets limits that are less"
                                                                    " restrictive than the minimum=" +
                    Limits<T>(this->get_min(), this->get_max()).str();
                throw std::runtime_error(error);
            }
            _limits_ptr = limits;
            _limiter = std::make_unique<Limiter>(*limits);
        }
    }
    void set_transform(std::shared_ptr<const Transform<T>> transform) {
        if (transform == nullptr)
        {
            _transformer = std::make_unique<TransformerNone>(*transform, _value);
        } else {
            _transform_ptr = transform;
            _transformer = std::make_unique<TransformerNonNull>(*transform);
        }
    }
    void set_value(T value) {
        if (!(get_limits().check(value)))
            throw std::runtime_error("Value=" + std::to_string(value)
                                        + "!<> get_limits()=" + get_limits().str());
        _value = value;
        _transformer->set_value_untransformed(value);
    };
    void set_value_transformed(T value_transformed) {
        _transformer->set_value_transformed(value_transformed);
        set_value(_transformer->get_value_untransformed());
    }

    std::string str() const {
        return get_type_name() + "(" + std::to_string(_value) + "," + get_limits().str() + ")";
    }

    static constexpr const Limits<T> limits_maximal = Limits<T>(get_min(), get_max(), type_name<C>(), ".limits_maximal");
    static constexpr const UnitTransform<T> transform_none = UnitTransform<T>();

    Parameter(T value = get_default(), std::shared_ptr<const Limits<T>> limits = nullptr,
                std::shared_ptr<const Transform<T>> transform = nullptr)
    {
        set_limits(limits);
        set_transform(transform);
        set_value(value);
    }
    ~Parameter() {};
};
}
#endif //PARAMETERS_PARAMETER_H
