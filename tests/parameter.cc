//
// Created by dtaranu on 4/8/21.
//

#include <cfloat>
#include <iostream>

#ifndef PARAMETERS_PARAMETER_H
#include "parameter.h"
#endif

#ifndef PARAMETERS_TRANSFORMS_H
#include "transforms.h"
#endif

using namespace parameters;

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

int main()
{
    std::cout << "Testing standard parameters" << std::endl;
    auto real = RealParameter();
    auto nonneg = NonNegativeParameter();
    std::cout << nonneg.str() << "," << nonneg.get_name() << "," << nonneg.get_desc() << std::endl;
    auto pos = PositiveParameter();

    return EXIT_SUCCESS;
}