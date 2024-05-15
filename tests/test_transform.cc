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

#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN

#include "doctest.h"

#include "transforms.h"

namespace mod_params = lsst::modelfit::parameters;

TEST_CASE("Log10Transform") {
    auto transform = mod_params::Log10Transform<double>();
    CHECK_EQ(transform.forward(1.0), 0.);
    CHECK_LT(std::abs(transform.forward(10.0) - 1.), 1e-12);
    CHECK_EQ(transform.repr(), "lsst::modelfit::parameters::Log10Transform<double>()");
    CHECK_EQ(transform.repr(true, "."), "lsst.modelfit.parameters.Log10Transform<double>()");
    CHECK_EQ(transform.str(), "Log10Transform<double>()");
}

TEST_CASE("UnitTransform") {
    auto transform = mod_params::UnitTransform<double>();
    CHECK_EQ(transform.forward(1.), 1.);
    CHECK_EQ(transform.reverse(-1.), -1.);
    CHECK_EQ(transform.repr(), "lsst::modelfit::parameters::UnitTransform<double>()");
    CHECK_EQ(transform.repr(true, "."), "lsst.modelfit.parameters.UnitTransform<double>()");
    CHECK_EQ(transform.str(), "UnitTransform<double>()");
}
