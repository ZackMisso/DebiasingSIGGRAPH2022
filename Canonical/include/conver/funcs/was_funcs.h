#include <conver/simple_funcs.h>

struct WASSimpleFunc : public Function
{
    WASSimpleFunc(Float a, Float b, Float kappa) : Function(a, b), kappa(kappa) {}

    virtual Float eval(Float x) const
    {
        return 0.5 + exp(kappa * (1.0 - cos(x))) - 1.0 + cos(x);
    }

    virtual std::string name() const { return "was_simple"; }

    Float kappa;
};