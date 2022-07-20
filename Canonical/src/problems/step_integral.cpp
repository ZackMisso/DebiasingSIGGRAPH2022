#include <conver/problem.h>

StepIntegral::StepIntegral(Function *func,
                           Float x,
                           Float val_1,
                           Float val_2,
                           Float min_x,
                           Float max_x)
    : func(func), x(x), val_1(val_1), val_2(val_2), min_x(min_x), max_x(max_x)
{
    bounds = new Bounds1Df(func->a, func->b);
}

StepIntegral::~StepIntegral() {}

Float StepIntegral::eval(Float sample) const
{
    return (sample < x) ? val_1 : val_2;
}

Float StepIntegral::eval(const std::vector<Float> &samples) const
{
    Float mean = 0.0;

    for (int i = 0; i < samples.size(); ++i)
    {
        mean += eval(samples[i]);
    }

    mean /= Float(samples.size());

    return mean;
}

Float StepIntegral::eval_integral(Float sample) const
{
    return func->eval(sample) * (func->b - func->a);
}

Float StepIntegral::eval_integral(const std::vector<Float> &samples) const
{
    Float sum = 0.0;

    for (int i = 0; i < samples.size(); ++i)
    {
        sum += eval_integral(samples[i]);
    }

    return sum / Float(samples.size());
}

Float StepIntegral::eval_function(Float sample) const
{
    return func->eval(sample);
}

Float StepIntegral::expected() const
{
    if (min_x > x)
        return (max_x - min_x) * val_2;
    else if (max_x < x)
        return (max_x - min_x) * val_1;
    else
        return (x - min_x) * val_1 + (max_x - x) * val_2;
}

Float StepIntegral::expected_integral() const
{
    return func->integrate();
}