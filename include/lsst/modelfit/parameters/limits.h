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

#ifndef LSST_MODELFIT_PARAMETERS_LIMITS_H
#define LSST_MODELFIT_PARAMETERS_LIMITS_H

#include <cctype>
#include <limits>
#include <stdexcept>
#include <string_view>

#include "object.h"
#include "transform.h"
#include "type_name.h"

namespace lsst::modelfit::parameters {

/**
 * Range-based limits for parameter values.
 *
 * @tparam T The type of the value. Only floating point values are tested.
 */
template <typename T>
class Limits : public Object {
private:
    T _min;
    T _max;

    constexpr void _check(const T& min, const T& max) const {
        if (std::isnan(min) || std::isnan(max))
            throw std::runtime_error(str() + " can't be initialized with NaN limits");
        if (!(min <= max)) throw std::invalid_argument(str() + " can't be initialized with !(min <= max)");
    }

    inline void _check_min(const T& min) const {
        if (std::isnan(min)) throw std::invalid_argument(str() + " set_min given NaN");
        if (!(min <= _max)) throw std::invalid_argument(str() + " set_min !(min_new <= max)");
    }
    inline void _check_max(const T& max) const {
        if (std::isnan(max)) throw std::invalid_argument(str() + " set_max given NaN");
        if (!(max >= _min)) throw std::invalid_argument(str() + " set_max !(min <= max_new)");
    }

public:
    /// Check if a value is within the limits
    inline bool check(T value) const { return value >= _min && value <= _max; }
    /// Return the closest value to the input that is within the limits
    inline T clip(T value) const { return value > _max ? _max : (value < _min ? _min : value); }

    /// Return the minimum
    inline T get_min() const { return _min; };
    /// Return the maximum
    inline T get_max() const { return _max; };

    std::string name;

    /// Set the minimum and maximum
    void set(T min, T max) {
        _check(min, max);
        _min = min;
        _max = max;
    }
    /// Set the minimum
    void set_min(T min) {
        _check_min(min);
        _min = min;
    };
    /// Set the maximum
    void set_max(T max) {
        _check_max(max);
        _max = max;
    };

    std::string repr(bool name_keywords = false) const override {
        return "Limits"
               + (name_keywords ? std::to_string(std::toupper(std::string(type_name<T>())[0]))
                                : "<" + std::string(type_name<T>()) + ">")
               + "(" + (name_keywords ? "min=" : "") + std::to_string(_min) + ", "
               + (name_keywords ? "max=" : "") + std::to_string(_max) + ", " + (name_keywords ? "name='" : "")
               + name + "')";
    }
    std::string str() const override {
        return "Limits(" + std::to_string(_min) + ", " + std::to_string(_max) + ", '" + name + "')";
    }

    /// Initialize limits from the minimum and maximum value.
    Limits(T min = -std::numeric_limits<T>::infinity(), T max = std::numeric_limits<T>::infinity(),
           std::string name_ = "")
            : _min(min), _max(max), name(name_) {
        _check(min, max);
    }

    ~Limits(){};
};

}  // namespace lsst::modelfit::parameters
#endif  // LSST_MODELFIT_PARAMETERS_LIMITS_H
