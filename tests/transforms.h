//
// Created by dtaranu on 4/8/21.
//

#ifndef PARAMETERS_TRANSFORMS_H
#define PARAMETERS_TRANSFORMS_H

#include <cmath>

#ifndef PARAMETERS_TRANSFORM_H
#include "transform.h"
#endif

using namespace parameters;

template <typename T>
class LogTransform : public Transform<T>
{
    std::string description() const { return "Natural (base e) logarithmic transform";}
    std::string str() const { return "LogTransform";}

    inline T forward(T x) const { return log(x);}
    inline T reverse(T x) const { return exp(x);}
};

template <typename T>
class Log10Transform : public Transform<T>
{
    std::string description() const { return "Base 10 logarithmic transform";}
    std::string str() const { return "Log10Transform";}

    inline T forward(T x) const { return log10(x);}
    inline T reverse(T x) const { return pow10(x);}
};

#endif //PARAMETERS_TRANSFORMS_H
