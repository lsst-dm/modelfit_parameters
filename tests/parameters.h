//
// Created by dtaranu on 4/8/21.
//

#ifndef PARAMETERS_TRANSFORMS_H
#define PARAMETERS_TRANSFORMS_H

#include <cfloat>
#include <cmath>
#include "parameter.h"

#ifndef PARAMETERS_TRANSFORM_H
#include "transform.h"
#endif

using namespace parameters;

static const std::string name_unit_none = "None";
class UnitNone : public parameters::Unit {
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

#endif  // PARAMETERS_TRANSFORMS_H
