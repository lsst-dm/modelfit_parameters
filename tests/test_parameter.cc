#include "transform.h"
#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN

#include "doctest.h"

#include "parameters.h"
#include "transforms.h"

TEST_CASE("RealParameter") {
    auto transform = std::make_shared<UnitTransform<double>>();
    auto unit = std::make_shared<UnitNone>();

    auto real = RealParameter(0, nullptr, transform);
    real.set_unit(unit);
    CHECK(real.get_unit().get_name() == unit->get_name());
    CHECK(real.get_value() == 0);
    CHECK(real.repr().size() > 0);
    CHECK(real.str().size() > 0);
}

TEST_CASE("NonNegativeParameter") {
    auto nonneg = NonNegativeParameter();
    CHECK(nonneg.get_min() == 0);
}

TEST_CASE("NonNegativeParameter") { auto pos = PositiveParameter(); }
