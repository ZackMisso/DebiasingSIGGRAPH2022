#include <conver/problem.h>

SimpleFD::SimpleFD(Function *func, Float pt) : func(func), pt(pt)
{
    // bounds should not really be used here
    // bounds = new SimpleFD(optical_depth->a, optical_depth->b);
}

SimpleFD::~SimpleFD()
{
    // should this object own the function? Probably not
    // delete optical_depth;
}

Float SimpleFD::eval(double sample) const
{
    // sample will incode the distance from pt, so this will return (f(pt+h) - f(pt)) / h
    return (func->eval(pt + sample) - func->eval(pt)) / sample;
}

Float SimpleFD::eval(const std::vector<Float> &samples) const
{
    throw new NotImplementedException("simple fd eval samples");
}

Float SimpleFD::expected() const
{
    return func->eval_deriv(pt);
}