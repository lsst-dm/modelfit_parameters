#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN

#include "doctest.h"

#include <limits>

#include "limits.h"

TEST_CASE("Limits") {
    double inf = std::numeric_limits<double>::infinity();
    auto limits_full = parameters::Limits<double>();
    CHECK(limits_full.check(-inf) == true);
    CHECK(limits_full.check(inf) == true);
    CHECK(limits_full.get_min() == -inf);
    CHECK(limits_full.get_max() == inf);
    auto limits_minmax = parameters::Limits<double>(std::numeric_limits<double>::min(),
                                                    std::numeric_limits<double>::max());
    CHECK(limits_minmax.check(-inf) == false);
    CHECK(limits_minmax.check(inf) == false);
    auto limits = parameters::Limits<double>(0, 0, "zero");
    CHECK(limits.check(0) == true);
    CHECK(limits.check(-1) == false);
    CHECK(limits.check(1) == false);
    CHECK(limits.clip(-1) == 0);
    CHECK(limits.clip(1) == 0);
    CHECK(limits.repr().size() > 0);
    CHECK(limits.str().size() > 0);
}
