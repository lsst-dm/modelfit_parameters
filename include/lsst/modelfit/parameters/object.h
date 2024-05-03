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

#ifndef LSST_MODELFIT_PARAMETERS_OBJECT_H
#define LSST_MODELFIT_PARAMETERS_OBJECT_H

#include <string>

namespace lsst::modelfit::parameters {

/**
 * A generic object from the parameters library.
 *
 * Objects have string representations that are guaranteed to be valid in C++
 * and should also be valid in Python bindings, if implemented correctly.
 * The interface may be expanded in the future.
 */
class Object {
public:
    /**
     * Return a full, callable string representation of this.
     *
     * @param name_keywords Whether to prefix arguments with "{name}=",
     *      where name is the arg name in the header (as with keyword
     *      arguments in Python).
     * @param namespace_separator The string to use to delimit namespaces,
     *      i.e. :: in C++ and . in Python.
     * @return
     *      A callable string representation of this, which should return an
     *      an identical object to this.
     *
     * @note The representation with name_keywords=false must be callable
     * in C++. The representation with name_keywords=true should be callable
     * in Python, if there are any bindings.
     */
    virtual std::string repr(bool name_keywords = false) const = 0;
    /// Return a brief, human-readable string representation of this.
    virtual std::string str() const = 0;

    friend std::ostream &operator<<(std::ostream &out, const Object &obj) {
        out << obj.str();
        return out;
    }

    virtual ~Object() = default;
};

}  // namespace lsst::modelfit::parameters
#endif
