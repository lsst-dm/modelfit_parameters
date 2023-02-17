#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN

#include "doctest.h"

#include "parameters.h"
#include "transforms.h"
#include "type_name.h"
#include "unit.h"

TEST_CASE("RealParameter")
{
    auto unit = std::make_shared<UnitNone>();
    auto real = RealParameter();
    real.set_unit(unit);
    CHECK(real.get_unit().get_name() == unit->get_name());
}

TEST_CASE("NonNegativeParameter")
{
    auto nonneg = NonNegativeParameter();
    CHECK(nonneg.get_min() == 0);
}

TEST_CASE("NonNegativeParameter")
{
    auto pos = PositiveParameter();
}
