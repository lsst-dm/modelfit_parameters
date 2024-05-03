// -*- LSST-C++ -*-
/*
 * This file is part of modelfit_parameters.
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

#ifndef LSST_MODELFIT_PARAMETERS_PARAMETER_H
#define LSST_MODELFIT_PARAMETERS_PARAMETER_H

#include <cmath>
#include <iostream>
#include <limits>
#include <memory>
#include <set>
#include <stdexcept>
#include <string>

#include "limits.h"
#include "object.h"
#include "transform.h"
#include "type_name.h"
#include "unit.h"

namespace lsst::modelfit::parameters {

/**
 * @brief Interface for parameters with values and metadata.
 *
 * A parameter represents a single numeric value, with metadata like a name,
 * description, unit, as well as optional limits and a transforming function.
 * This class is intended for use in numerical optimization.
 *
 * @tparam T The type of the value. Only floating point values are tested.
 *
 * @note This base class exists partly because e.g. Python bindings can't
 * understand CRTP. It is probably unnecessary (albeit convenient) in C++
 * and comes with a performance cost.
 */
template <typename T>
class ParameterBase : public Object {
public:
    /// Get the default value.
    virtual T get_default() const = 0;
    /// Get a string description for this parameter class.
    virtual std::string get_desc() const = 0;
    /// Return whether the parameter is fixed (not free).
    virtual bool get_fixed() const = 0;
    /// Return whether the parameter is free (not fixed).
    virtual bool get_free() const = 0;
    /// Return a string label for this parameter instance.
    virtual std::string get_label() const = 0;
    /// Return the limits for the untransformed value.
    virtual const Limits<T>& get_limits() const = 0;
    /// Return limits representing the maximum/minimum untransformed value.
    virtual const Limits<T>& get_limits_maximal() const = 0;
    /// Return whether the parameter is linear.
    virtual bool get_linear() const = 0;
    /// Return the minimum value for this parameter instance.
    virtual T get_min() const = 0;
    /// Return the maximum value for this parameter instance.
    virtual T get_max() const = 0;
    /// Get a string name for this parameter class.
    virtual std::string get_name() const = 0;
    /// Return the transforming function for this parameter instance.
    virtual const Transform<T>& get_transform() const = 0;
    /// Return the derivative of the transform for this parameter instance.
    virtual T get_transform_derivative() const = 0;
    /// Return the transform pointer for this parameter instance.
    virtual std::shared_ptr<const Transform<T>> get_transform_ptr() const = 0;
    /// Return the untransformed value of this parameter instance.
    virtual T get_value() const = 0;
    /// Return the transformed value of this parameter instance.
    virtual T get_value_transformed() const = 0;
    /// Return the unit of this parameter instance.
    virtual const Unit& get_unit() const = 0;
    /// Set the parameter to be fixed (or not).
    virtual void set_fixed(bool fixed) = 0;
    /// Set the parameter to be free (or not).
    virtual void set_free(bool free) = 0;
    /// Set the string label for this parameter instance.
    virtual void set_label(std::string label) = 0;
    /// Set the limits for this parameter instance.
    virtual void set_limits(std::shared_ptr<const Limits<T>> limits) = 0;
    /// Set the transforming function for this parameter instance.
    virtual void set_transform(std::shared_ptr<const Transform<T>> transform) = 0;
    /// Set the untransformed value for this parameter instance.
    virtual void set_value(T value) = 0;
    /// Set the transformed value for this parameter instance.
    virtual void set_value_transformed(T value_transformed) = 0;
    /// Set the unit for this parameter instance.
    virtual void set_unit(std::shared_ptr<const Unit> unit = nullptr) = 0;

    static const UnitTransform<T>& transform_none() { return UnitTransform<T>::get(); };

    friend bool operator==(const ParameterBase<T>& first, const ParameterBase<T>& second) {
        return &first == &second;
    }

    friend bool operator!=(const ParameterBase<T>& first, const ParameterBase<T>& second) {
        return &first != &second;
    }

    friend bool operator<(const ParameterBase<T>& first, const ParameterBase<T>& second) {
        return std::less<const ParameterBase<T>*>{}(&first, &second) == true;
    }

    virtual ~ParameterBase() = default;
};

/**
 * @brief A parameter with values and metadata.
 *
 * This is a CRTP implementation of the ParameterBase interface, which allows
 * for concise, minimal-effort derived classes (see tests and examples).
 *
 * @tparam T The type of the value. Only floating point values are tested.
 * @tparam C The derived class.
 *
 * @note Parameters can be set as free/fixed when they are part of a model.
 * This designation is indicative and does not prevent the parameter value
 * from being changed by users.
 *
 * @note CRTP performance benefits are likely lost by having an abstract base
 * class with virtual methods; see ParameterBase Notes. This may be obviated
 * by future compiler improvements.
 *
 * @note The main benefit of CRTP left is that derived classes can be
 * implemented just by defining static members. The implementation thereof in
 * this class is not trivial, however.
 */
template <typename T, class C>
class Parameter : public ParameterBase<T>, public std::enable_shared_from_this<C> {
private:
    // These classes can store the default const Limits/Transform refs,
    // in case the unique_ptr thereof is null.

    /// A container for a const Limits ref
    struct Limiter {
        const Limits<T>& limits;
        Limiter(const Limits<T>& limits_in) : limits(limits_in){};
    };

    /// A container for a const Transform ref
    struct Transformer {
        const Transform<T>& transform;
        Transformer(const Transform<T>& transform_in) : transform(transform_in){};
    };

    /// The default value for this type of Parameter
    static constexpr T _default = 0;
    /// The minimum valid value (inclusive) for this type of Parameter
    static constexpr T _min = -std::numeric_limits<T>::infinity();
    /// The maximum valid value (inclusive) for this type of Parameter
    static constexpr T _max = std::numeric_limits<T>::infinity();
    /// Whether this Parameter is a linear parameter in a model
    static constexpr bool _linear = false;
    /// Whether this Parameter is a free parameter in a model
    bool _free = true;
    /// A Limiter to further restrict this parameter's values
    std::unique_ptr<Limiter> _limiter;
    /// A Transformer to remap this parameter's value
    std::unique_ptr<Transformer> _transformer;
    /// A descriptive label for this parameter
    std::string _label;
    /// The Limits for this parameter, if not default
    std::shared_ptr<const Limits<T>> _limits_ptr;
    /// The Transform for this parameter, if not default
    std::shared_ptr<const Transform<T>> _transform_ptr;
    /// The Unit for this parameter's untransformed value
    std::shared_ptr<const Unit> _unit_ptr;

    /// Set the untransformed value, checking limits
    void _set_value(T value) {
        if (!(get_limits().check(value)))
            throw std::runtime_error(this->str() + "Value=" + std::to_string(value)
                                     + " beyond get_limits()=" + get_limits().str());
        _value = value;
    }

protected:
    /// The untransformed value
    T _value;
    /// The cached, transformed value
    T _value_transformed;

    static const std::string _get_desc() { return C::_desc; }
    static constexpr bool _get_linear() { return C::_linear; }
    static constexpr T _get_min() { return C::_min; }
    static constexpr T _get_max() { return C::_max; }
    static const std::string _get_name() { return C::_name; }

public:
    /// Get the default value for the derived type of this
    static constexpr T _get_default() { return C::_default; }

    std::string get_desc() const override { return _get_desc(); }

    T get_default() const override { return _get_default(); }

    bool get_fixed() const override { return !_free; }

    bool get_free() const override { return _free; }

    std::string get_label() const override { return _label; }

    const Limits<T>& get_limits_maximal() const override {
        static const Limits<T> limits_maximal
                = Limits<T>(_get_min(), _get_max(), std::string(type_name<C>()) + ".limits_maximal");
        return limits_maximal;
    }

    const Limits<T>& get_limits() const override { return _limiter->limits; }

    bool get_linear() const override { return _get_linear(); }

    T get_min() const override { return _get_min(); }

    T get_max() const override { return _get_max(); }

    std::string get_name() const override { return _get_name(); }

    const Transform<T>& get_transform() const override { return _transformer->transform; }

    std::shared_ptr<const Transform<T>> get_transform_ptr() const override { return _transform_ptr; }

    T get_transform_derivative() const override {
        return this->get_transform().derivative(this->get_value());
    }
    /// Get the name of the derived type of this
    static const std::string get_type_name() { return std::string(type_name<C>()); }

    const Unit& get_unit() const override { return *_unit_ptr; }

    T get_value() const override { return _value; }

    T get_value_transformed() const override { return _value_transformed; }

    /// Return a shared pointer to this
    std::shared_ptr<C> ptr() { return this->shared_from_this(); }

    void set_fixed(bool fixed) override { set_free(!fixed); }
    void set_free(bool free) override { _free = free; }
    void set_label(std::string label) override { _label = std::move(label); }
    void set_limits(std::shared_ptr<const Limits<T>> limits) override {
        // TODO: Fix bad_alloc when calling this without &
        // Disable copy constructor explicitly maybe?
        const auto& limits_maximal = this->get_limits_maximal();
        if (limits == nullptr) {
            _limiter = std::make_unique<Limiter>(limits_maximal);
        } else {
            if (!((limits->get_min() >= this->get_min()) && (limits->get_max() <= this->get_max()))) {
                std::string error = get_type_name() + ".set_limits(" + limits->str()
                                    + ") sets limits that are less restrictive than the minimum="
                                    + limits_maximal.str();
                throw std::runtime_error(error);
            }
            _limits_ptr = std::move(limits);
            _limiter = std::make_unique<Limiter>(*_limits_ptr);
        }
    }
    void set_transform(const std::shared_ptr<const Transform<T>> transform) override {
        if (transform == nullptr) {
            // TODO: determine why passing transform_none as arg here returns:
            // error: modification of '<temporary>' is not a constant expression
            // whereas get_transform_unit<T>() results in a segfault
            // (iff Transform has a virtual destructor)
            _transformer = std::make_unique<Transformer>(this->transform_none());
        } else {
            _transform_ptr = std::move(transform);
            _transformer = std::make_unique<Transformer>(*_transform_ptr);
        }
        _value_transformed = _transformer->transform.forward(_value);
    }

    void set_value(T value) override {
        _set_value(value);
        double value_new = this->get_value();
        _value_transformed = _transformer->transform.forward(value_new);
    };

    void set_value_transformed(T value_transformed) override {
        _set_value(_transformer->transform.reverse(value_transformed));
        _value_transformed = _transformer->transform.forward(this->get_value());
    }

    void set_unit(std::shared_ptr<const Unit> unit = nullptr) override {
        _unit_ptr = unit == nullptr ? nullptr : std::move(unit);
    }

    std::string repr(bool name_keywords = false) const override {
        return get_type_name() + "(" + (name_keywords ? "value=" : "") + std::to_string(_value) + ", "
               + (name_keywords ? "limits=" : "") + get_limits().repr() + ", "
               + (name_keywords ? "transform=" : "") + get_transform().repr() + ", "
               + (name_keywords ? "fixed=" : "") + std::to_string(0 + get_fixed()) + ", "
               + (name_keywords ? "label='" : "'") + _label + "')";
    }

    std::string str() const override {
        return get_type_name() + "(value=" + std::to_string(_value)
               + ", "
               // TODO: Implement equality operators for limits/transforms
               + ((&get_limits() == &get_limits_maximal()) ? "" : ("limits=" + get_limits().repr() + ", "))
               + ((&get_transform() == &(this->transform_none()))
                          ? ""
                          : ("transform=" + get_transform().repr() + ", "))
               + (!get_fixed() ? "" : (std::string("fixed=") + std::to_string(0 + get_fixed()) + ", "))
               + ((get_label() == "") ? "" : ("label='" + get_label() + "'")) + ")";
    }

    /**
     * Initialize a Parameter.
     *
     * @param value The initial untransformed value.
     * @param limits The untransformed value limits.
     * @param transform The transformation to apply to values.
     * @param unit The unit of the untransformed value.
     * @param fixed Whether the parameter is fixed in models.
     * @param label A descriptive label for the parameter.
     */
    Parameter(T value = _get_default(), std::shared_ptr<const Limits<T>> limits = nullptr,
              const std::shared_ptr<const Transform<T>> transform = nullptr,
              std::shared_ptr<const Unit> unit = nullptr, bool fixed = false, std::string label = "")
            : ParameterBase<T>() {
        set_limits(limits);
        _value = value;
        set_transform(transform == nullptr ? nullptr : std::move(transform));
        set_unit(unit);
        set_fixed(fixed);
        set_label(label);
    }
    ~Parameter(){};
};
}  // namespace lsst::modelfit::parameters
#endif  // LSST_MODELFIT_PARAMETERS_PARAMETER_H
