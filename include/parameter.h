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
#include <set>
#include <stdexcept>
#include <string>

#include "limits.h"
#include "transform.h"
#include "type_name.h"
#include "unit.h"

namespace parameters {

template<typename T>
class ParameterBase {
public:
    virtual T get_default() const = 0;
    virtual std::string get_desc() const = 0;
    virtual bool get_fixed() const = 0;
    virtual bool get_free() const = 0;
    virtual std::string get_label() const = 0;
    virtual const Limits<T> & get_limits() const = 0;
    virtual T get_min() const = 0;
    virtual T get_max() const = 0;
    virtual std::string get_name() const = 0;
    virtual const Transform<T> & get_transform() const = 0;
    virtual T get_transform_derivative() const = 0;
    virtual T get_value() const = 0;
    virtual T get_value_transformed() const = 0;
    virtual const Unit & get_unit() const = 0;

    virtual void set_fixed(bool fixed) = 0;
    virtual void set_free(bool free) = 0;
    virtual void set_label(std::string label) = 0;
    virtual void set_limits(std::shared_ptr<const Limits<T>> limits) = 0;
    virtual void set_transform(const std::shared_ptr<const Transform<T>> transform) = 0;
    virtual void set_value(T value) = 0;
    virtual void set_value_transformed(T value_transformed) = 0;
    virtual void set_unit(std::shared_ptr<const Unit> unit = nullptr) = 0;
    virtual std::string str() const = 0;

    friend bool operator == (const ParameterBase<T> & first, const ParameterBase<T> & second) {
        return &first == &second;
    }

    friend bool operator != (const ParameterBase<T> & first, const ParameterBase<T> & second) {
        return !(first == second);
    }

    friend bool operator < (const ParameterBase<T> & first, const ParameterBase<T> & second) {
        return &first < &second;
    }

    virtual ~ParameterBase() = default;
};

template<typename T, class C>
class Parameter : public ParameterBase<T>, public std::enable_shared_from_this<C> {
public:
    using SetC = std::set<std::shared_ptr<C>>;

private:
    // TODO: Remember why this class needs to exist, if it does
    // Probably just to store the const Limits reference?
    struct Limiter {
        const Limits<T> & limits;
        Limiter(const Limits<T> & limits_in) : limits(limits_in) {};
    };

    struct Transformer {
        const Transform<T> & transform;
        Transformer(const Transform<T> & transform_in) : transform(transform_in) {};
    };

    static constexpr T _default = 0;
    static constexpr T _min = -std::numeric_limits<T>::infinity();
    static constexpr T _max = std::numeric_limits<T>::infinity();

    bool _free = true;
    std::unique_ptr<Limiter> _limiter;
    std::unique_ptr<Transformer> _transformer;

    std::string _label;
    SetC _inheritors;
    SetC _modifiers;

    std::shared_ptr<const Limits<T>> _limits_ptr;
    std::shared_ptr<const Transform<T>> _transform_ptr;
    std::shared_ptr<const Unit> _unit_ptr;
    std::shared_ptr<const C> _inheritee_ptr = nullptr;

    void _set_value(T value)
    {
        if (!(get_limits().check(value))) throw std::runtime_error(
            "Value=" + std::to_string(value) + "beyond get_limits()=" + get_limits().str()
        );
        _value = value;
    }

protected:
    T _value;
    T _value_transformed;

    void set_inheritee(std::shared_ptr<const C> inheritee) {
        _inheritee_ptr = inheritee == nullptr ? nullptr : std::move(inheritee);
    }

public:
    static constexpr T _get_default() { return C::_default; }
    static const std::string _get_desc() { return C::_desc; }
    static constexpr T _get_min() { return C::_min; }
    static constexpr T _get_max() { return C::_max; }
    static const std::string _get_name() { return C::_name; }

    void add_inheritor(std::shared_ptr<C> inheritor) { 
        if(inheritor == nullptr) throw std::runtime_error("Can't add null inheritor to " + this->str());
        if(!inheritor->get_free()) throw std::runtime_error("Can't add_inheritor(" + inheritor->str()
            + ") with fixed inheritor");
        if(!inheritor->get_inheritors().empty()) throw std::runtime_error("Can't add_inheritor("
            + inheritor->str() + ") with inheritors of its own; inheritance may not be nested");
        _inheritors.insert(inheritor);
    }
    void add_modifier(std::shared_ptr<C> modifier) { 
        if(modifier == nullptr) throw std::runtime_error("Can't add null modifier to " + this->str());
        if(_modifiers == nullptr) _modifiers = std::make_unique<SetC>();
        _modifiers.insert(modifier);
    }

    std::string get_desc() const override { return _get_desc(); }
    T get_default() const override { return _get_default(); }
    bool get_fixed() const override { return !_free; }
    bool get_free() const override { return _free; }
    std::shared_ptr<const C> get_inheritee() const { return _inheritee_ptr; }
    SetC get_inheritors() const { return _inheritors; }
    std::string get_label() const override { return _label; }
    const Limits<T> & get_limits() const override { return _limiter->limits; }
    T get_min() const override { return _get_min(); }
    T get_max() const override { return _get_max(); }
    SetC get_modifiers() const { return _modifiers; }
    std::string get_name() const override { return _get_name(); }
    const Transform<T> & get_transform() const override { return _transformer->transform; }
    T get_transform_derivative() const override {
        return this->get_transform().derivative(this->get_value());
    }
    static const std::string get_type_name() { return std::string(type_name<C>()); }
    const Unit & get_unit() const override { return *_unit_ptr; }
    T get_value() const override { return _value; }
    T get_value_transformed() const override { return _value_transformed; }

    static constexpr const Limits<T> limits_maximal = Limits<T>(_get_min(), _get_max(),
        type_name<C>(), ".limits_maximal");

    std::shared_ptr<C> ptr() { return this->shared_from_this(); }

    void remove_inheritor(std::shared_ptr<C> inheritor) {
        if(inheritor == nullptr) throw std::runtime_error("Can't remove null inheritor from " + this->str());
        _inheritors->erase(inheritor); 
    }

    void remove_modifier(std::shared_ptr<C> modifier) {
        if(modifier == nullptr) throw std::runtime_error("Can't remove null modifier from " + this->str());
        _modifiers->erase(modifier); 
    }

    void set_fixed(bool fixed) override { set_free(!fixed); }
    void set_free(bool free) override {
        if(_inheritee_ptr != nullptr) "Can't set this=" + this->str() + " free while it inherits from "
            "inheritee = " + _inheritee_ptr->str();
        _free = free;
    }
    void set_label(std::string label) override { _label = std::move(label); }
    void set_inheritors(const SetC inheritors)
    {
        _inheritors = inheritors;
    }
    void set_limits(std::shared_ptr<const Limits<T>> limits) override {
        if (limits == nullptr) 
        {
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
    void set_modifiers(const SetC modifiers)
    {   
        _modifiers = modifiers;
    }
    void set_transform(const std::shared_ptr<const Transform<T>> transform) override {
        if(transform == nullptr) {
            // TODO: determine why passing transform_none as arg here returns:
            // error: modification of '<temporary>' is not a constant expression
            // whereas get_transform_unit<T>() results in a segfault
            // (iff Transform has a virtual destructor)
            _transformer = std::make_unique<Transformer>(transform_none);
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
        for(auto & inheritor : _inheritors) inheritor->set_value(value_new);
    };
    
    void set_value_transformed(T value_transformed) override {
        _set_value(_transformer->transform.reverse(value_transformed));
        _value_transformed = _transformer->transform.forward(this->get_value());
        for(auto & inheritor : _inheritors) inheritor->set_value_transformed(_value_transformed);
    }

    void set_unit(std::shared_ptr<const Unit> unit = nullptr) override {
        _unit_ptr = unit == nullptr ? nullptr : std::move(unit);
    }

    std::string str() const override {
        return get_type_name() + "(" + std::to_string(_value) + "," + get_limits().str() + ")";
    }

    static constexpr const UnitTransform<T> & transform_none = get_transform_unit<T>();

    Parameter(
        T value = _get_default(),
        std::shared_ptr<const Limits<T>> limits = nullptr,
        const std::shared_ptr<const Transform<T>> transform = nullptr,
        std::shared_ptr<const Unit> unit = nullptr,
        bool fixed = false,
        std::string label = "",
        const SetC & inheritors = {},
        const SetC & modifiers = {}
    ) : ParameterBase<T>() {
        set_limits(limits);
        _value = value;
        set_transform(transform == nullptr ? nullptr : std::move(transform));
        // inheritors need to go before value, which is set by set_transform
        set_inheritors(inheritors);
        set_modifiers(modifiers);
        set_unit(unit);
        set_fixed(fixed);
        set_label(label);
    }
    ~Parameter() {};
};
}
#endif //PARAMETERS_PARAMETER_H
