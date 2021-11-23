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

#ifndef PARAMETERS_LIMITS_H
#define PARAMETERS_LIMITS_H

#include <stdexcept>
#include <string_view>

#include "transform.h"
#include "type_name.h"

namespace parameters {

static const std::string _limits_name_default = "Default";

template<typename T>
class Limits {
private:
    T _min;
    T _max;
    const std::string_view _name = _limits_name_default;
    const std::string_view _suffix = "";


    constexpr void _check(const T & min, const T & max) const {
        if (std::isnan(min) || std::isnan(max)) throw std::runtime_error(
            str() + " can't be initialized with NaN limits");
        if (!(min <= max)) throw std::runtime_error(str() + " can't be initialized with !(min <= max)");
    }

    inline void _check_min(const T& min) const {
        if (std::isnan(min))
            throw std::runtime_error(str() + " set_min given NaN");
        if (!(min <= _max))
            throw std::runtime_error(str() + " set_min !(min_new <= max)");
    }
    inline void _check_max(const T& max) const {
        if (std::isnan(max))
            throw std::runtime_error(str() + " set_max given NaN");
        if (!(max >= _min))
            throw std::runtime_error(str() + " set_max !(min <= max_new)");
    }

public:
    inline bool check(T value) const { return value >= _min && value <= _max; }
    inline T clip(T value) const { return value > _max ? _max : (value < _min ? _min : value); }

    inline T get_min() const { return _min; };
    inline T get_max() const { return _max; };

    void set(T min, T max) {
        _check(min, max);
        _min = min;
        _max = max;
    }
    void set_min(T min) {
        _check_min(min);
        _min = min;
    };
    void set_max(T max) {
        _check_max(max);
        _max = max;
    };

    std::string str() const {
        return "parameters::Limits<" + std::string(type_name<T>()) + ">(min=" + std::to_string(_min)
            + ", max=" + std::to_string(_max) + ", name=" + std::string(_name) + std::string(_suffix) + ")";
    }

    Limits(T min = -std::numeric_limits<T>::infinity(), T max = -std::numeric_limits<T>::infinity(),
        const std::string & name=_limits_name_default) : _min(min), _max(max), _name(name) {
        _check(min, max);
    }
    // Used for static initialization of limits. Name/suffix exist because I can't figure out how to concatenate them
    constexpr Limits(const T & min, const T & max, const std::string_view & name, const std::string_view & suffix="") : 
        _min(min), _max(max), _name(name), _suffix(suffix) {
        _check(min, max);
    };
};

}
#endif //PARAMETERS_LIMITS_H
