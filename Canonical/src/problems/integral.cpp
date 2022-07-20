#include <conver/problem.h>

IntegralEval::IntegralEval(Function *func) : optical_depth(func)
{
    bounds = new Bounds1Df(optical_depth->a, optical_depth->b);
}

IntegralEval::~IntegralEval() {}

Float IntegralEval::eval(Float sample) const
{
    return eval_integral(sample);
}

Float IntegralEval::eval(const std::vector<Float> &samples) const
{
    return eval_integral(samples);
}

Float IntegralEval::eval_integral(Float sample) const
{
    return optical_depth->eval(sample) * (optical_depth->b - optical_depth->a);
}

Float IntegralEval::eval_integral(const std::vector<Float> &samples) const
{
    Float sum = 0.0;

    for (int i = 0; i < samples.size(); ++i)
    {
        sum += eval_integral(samples[i]);
    }

    return sum / Float(samples.size());
}

Float IntegralEval::eval_function(Float sample) const
{
    return optical_depth->eval(sample);
}

Float IntegralEval::expected() const
{
    return optical_depth->integrate();
}

Float IntegralEval::expected_integral() const
{
    return optical_depth->integrate();
}