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

#ifndef LSST_MODELFIT_PARAMETERS_TESTS_TRANSFORMS_H
#define LSST_MODELFIT_PARAMETERS_TESTS_TRANSFORMS_H

#include <cmath>
#include "lsst/modelfit/parameters/transform.h"

namespace lsst::modelfit::parameters {

template <typename T>
class LogTransform : public Transform<T> {
public:
    std::string description() const override { return "Natural (base e) logarithmic transform"; }
    std::string repr(bool = false,
                     const std::string_view& namespace_separator = Object::CC_NAMESPACE_SEPARATOR
                     ) const override {
        return type_name_str<LogTransform>(false, namespace_separator) + "()";
    }
    std::string str() const override {
        return type_name_str<LogTransform>(true) + "()";
    }

    inline T derivative(T x) const override { return 1./x; }
    inline T forward(T x) const override { return log(x); }
    inline T reverse(T x) const override { return exp(x); }
};

template <typename T>
class Log10Transform : public Transform<T> {
public:
    std::string description() const override { return "Base 10 logarithmic transform"; }
    std::string repr(bool = false,
                     const std::string_view& namespace_separator = Object::CC_NAMESPACE_SEPARATOR
                     ) const override {
        return type_name_str<Log10Transform>(false, namespace_separator) + "()";
    }
    std::string str() const override {
        return type_name_str<Log10Transform>(true) + "()";
    }

    inline T derivative(T x) const override { return 1./x; }
    inline T forward(T x) const override { return log10(x); }
    // pow10 is missing on macos
    inline T reverse(T x) const override { return pow(10.0, x); }
};
}

#endif  // LSST_MODELFIT_PARAMETERS_TESTS_TRANSFORMS_H
