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

#include "parameters.h"
#include "transforms.h"

namespace mod_params = lsst::modelfit::parameters;

TEST_CASE("RealParameter") {
    auto transform = std::make_shared<mod_params::UnitTransform<double>>();
    auto unit = std::make_shared<mod_params::UnitNone>();

    auto real = mod_params::RealParameter(0, nullptr, transform);
    real.set_unit(unit);
    CHECK_EQ(real.get_unit().get_name(), unit->get_name());
    CHECK_EQ(real.get_value(), 0);
    CHECK_GT(real.repr().size(), 0);
    CHECK_GT(real.str().size(), 0);

    CHECK_EQ(real.get_transform_ptr(), transform);

    auto real2 = mod_params::RealParameter(0, nullptr, transform);
    CHECK_NE(real, real2);
    CHECK_EQ(real == real2, false);
    // CHECK_NOTHROW unfortunately gives an unused result compiler warning
    CHECK_EQ((real < real2) | true, true);
    auto real_repr = real.repr();
    auto real_str = real.str();
    CHECK_EQ(real_repr.substr(0, 41), "lsst::modelfit::parameters::RealParameter");
    CHECK_EQ(real_str.substr(0, 13), "RealParameter");
}

TEST_CASE("NonNegativeParameter") {
    auto nonneg = mod_params::NonNegativeParameter();
    CHECK_EQ(nonneg.get_min(), 0);
}

TEST_CASE("PositiveParameter") { auto pos = mod_params::PositiveParameter(); }
