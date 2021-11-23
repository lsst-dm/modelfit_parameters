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

#ifndef PARAMETERS_TRANSFORM_H
#define PARAMETERS_TRANSFORM_H

#include <cmath>
#include <memory>
#include <string>

namespace parameters {

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wnon-virtual-dtor"
template <class T>
class Transform {
public:
    virtual std::string description() const = 0;
    virtual std::string str() const = 0;

    virtual T derivative(T x) const = 0;
    virtual T forward(T x) const = 0;
    virtual T reverse(T x) const = 0;
    // TODO: Find a solution for a destructor that compiles without warnings
    // gcc 9.4.0 gives:
    // ../include/transform.h:34:7: warning: 'class parameters::Transform<double>' has virtual
    // functions and accessible non-virtual destructor [-Wnon-virtual-dtor]
    // May not be possible; see http://www.open-std.org/jtc1/sc22/wg21/docs/papers/2018/p1077r0.html
    // virtual ~Transform() = default;
    // This results in (with gcc 9.4.0; line numbers may vary):
    // ../include/parameter.h:242:43: error: modification of '<temporary>' is not a constant expression
    //  242 |     static constexpr const Transform<T> & transform_none = get_transform_unit<T>();
    // = delete doesn't work either as it is actually called, hence giving use of deleted function errors.
    ~Transform() = default;
};

template <class T>
class UnitTransform : public Transform<T> {
public:
    std::string description() const { return "Unit transform";}
    std::string str() const { return "UnitTransform";}

    inline T derivative(T) const { return 1;}
    inline T forward(T x) const { return x;}
    inline T reverse(T x) const { return x;}
};

template <class T>
static constexpr UnitTransform<T> get_transform_unit()
{
    return UnitTransform<T>();
}
#pragma GCC diagnostic pop
}
#endif //PARAMETERS_TRANSFORM_H
