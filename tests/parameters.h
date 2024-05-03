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

#ifndef LSST_MODELFIT_PARAMETERS_TESTS_PARAMETERS_H
#define LSST_MODELFIT_PARAMETERS_TESTS_PARAMETERS_H

#include <cfloat>
#include <cmath>
#include "lsst/modelfit/parameters/parameter.h"

namespace lsst::modelfit::parameters {

static const std::string name_unit_none = "None";

/**
 * A bare-minimum implementation of a Unit with no name.
 */
class UnitNone : public Unit {
public:
    std::string get_name() const override { return name_unit_none; }
};

struct RealParameter : public Parameter<double, RealParameter> {
    static inline const std::string _desc = "Real, potentially infinite parameter";
    static inline const std::string _name = "real";
    using Parameter<double, RealParameter>::Parameter;
};

struct NonNegativeParameter : public Parameter<double, NonNegativeParameter> {
    static inline constexpr double _min = 0.;
    static inline const std::string _desc = "Non-negative, potentially infinite parameter";
    static inline const std::string _name = "non_negative";
    using Parameter<double, NonNegativeParameter>::Parameter;
};

struct PositiveParameter : public Parameter<double, PositiveParameter> {
    static inline constexpr double _min = DBL_TRUE_MIN;
    static inline const std::string _desc = "Non-negative, non-zero, potentially infinite parameter";
    static inline const std::string _name = "positive";
    using Parameter<double, PositiveParameter>::Parameter;
};
}

#endif  // LSST_MODELFIT_PARAMETERS_TESTS_PARAMETERS_H
