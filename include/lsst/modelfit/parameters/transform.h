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

#ifndef LSST_MODELFIT_PARAMETERS_TRANSFORM_H
#define LSST_MODELFIT_PARAMETERS_TRANSFORM_H

#include <cmath>
#include <memory>
#include <string>

#include "object.h"

namespace lsst::modelfit::parameters {

/**
 * @brief A reversible transformation of a real scalar value.
 *
 * The transformation should be differentiable, ideally analytically.
 *
 * @tparam T The type of the value. Only floating point values are tested.
 */
template <class T>
class Transform : public Object {
public:
    /// Return a description of this transform
    virtual std::string description() const = 0;

    /// Return the derivative of this tranform at the value x
    virtual T derivative(T x) const = 0;
    /// Return the transformed value of x
    virtual T forward(T x) const = 0;
    /// Return the original value of x given a transformed value
    virtual T reverse(T x) const = 0;

    virtual ~Transform() = default;
};

template <class T>
class UnitTransform : public Transform<T> {
public:
    std::string description() const override { return "Unit transform"; }
    static const UnitTransform<T>& get() {
        static const auto transform = UnitTransform<T>();
        return transform;
    }
    std::string repr(__attribute__((unused)) bool name_keywords = false) const override {
        return "UnitTransform()";
    }
    std::string str() const override { return "UnitTransform()"; }

    inline T derivative(T) const { return 1; }
    inline T forward(T x) const { return x; }
    inline T reverse(T x) const { return x; }

    ~UnitTransform() = default;
};

}  // namespace lsst::modelfit::parameters
#endif  // LSST_MODELFIT_PARAMETERS_TRANSFORM_H
