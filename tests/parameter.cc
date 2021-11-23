//
// Created by dtaranu on 4/8/21.
//

#include <cfloat>
#include <iostream>

#include "parameter.h"
#include "transforms.h"
#include "type_name.h"
#include "unit.h"

namespace parameters {
namespace {

static const std::string name_unit_none = "None";
class UnitNone : public Unit {
public:
    std::string get_name() const { return name_unit_none; }
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
    static inline const std::string _desc = "Non-negative, potentially infinite parameter";
    static inline const std::string _name = "positive";
    using Parameter<double, PositiveParameter>::Parameter;
};

int _main()
{
    std::cout << "Testing standard parameters" << std::endl;
    auto unit = std::make_shared<UnitNone>();
    auto real = RealParameter();
    real.set_unit(unit);
    auto nonneg = NonNegativeParameter();
    std::cout << nonneg.str() << "," << nonneg.get_name() << "," << nonneg.get_desc() << ","
        << nonneg.get_min() << std::endl;
    auto pos = PositiveParameter();
    std::cout << type_name<PositiveParameter>() << " == " << type_name_str<PositiveParameter>() << std::endl;

    return EXIT_SUCCESS;
}
static const int RESULT = _main();
}
}

int main() { return _main(); }