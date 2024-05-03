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

#include <limits>

#include "lsst/modelfit/parameters/limits.h"

namespace mod_params = lsst::modelfit::parameters;

TEST_CASE("Limits") {
    double inf = std::numeric_limits<double>::infinity();
    auto limits_full = mod_params::Limits<double>();
    CHECK_EQ(limits_full.check(-inf), true);
    CHECK_EQ(limits_full.check(inf), true);
    CHECK_EQ(limits_full.get_min(), -inf);
    CHECK_EQ(limits_full.get_max(), inf);
    auto limits_minmax = mod_params::Limits<double>(std::numeric_limits<double>::min(),
                                                    std::numeric_limits<double>::max());
    CHECK_EQ(limits_minmax.check(-inf), false);
    CHECK_EQ(limits_minmax.check(inf), false);
    auto limits = mod_params::Limits<double>(0, 0, "zero");
    CHECK_EQ(limits.check(0), true);
    CHECK_EQ(limits.check(-1), false);
    CHECK_EQ(limits.check(1), false);
    CHECK_EQ(limits.clip(-1), 0);
    CHECK_EQ(limits.clip(1), 0);
    CHECK_GT(limits.repr().size(), 0);
    CHECK_GT(limits.str().size(), 0);
}
