#include <conver/problem.h>

ExpTrans::ExpTrans(Function *func) : optical_depth(func)
{
    bounds = new Bounds1Df(optical_depth->a, optical_depth->b);
}

ExpTrans::~ExpTrans() {}

Float ExpTrans::eval(Float sample) const
{
    return exp(-sample);
}

Float ExpTrans::eval(const std::vector<Float> &samples) const
{
    Float mean = 0.0;

    for (int i = 0; i < samples.size(); ++i)
    {
        mean += eval(samples[i]);
    }

    mean /= Float(samples.size());

    return mean;
}

Float ExpTrans::eval_integral(Float sample) const
{
    return optical_depth->eval(sample) * (optical_depth->b - optical_depth->a);
}

Float ExpTrans::eval_integral(const std::vector<Float> &samples) const
{
    Float sum = 0.0;

    for (int i = 0; i < samples.size(); ++i)
    {
        sum += eval_integral(samples[i]);
    }

    return sum / Float(samples.size());
}

Float ExpTrans::eval_function(Float sample) const
{
    return optical_depth->eval(sample);
}

Float ExpTrans::expected() const
{
    return exp(-optical_depth->integrate());
}

Float ExpTrans::expected_integral() const
{
    return optical_depth->integrate();
}