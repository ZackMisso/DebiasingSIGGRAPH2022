#include <conver/problem.h>

Step::Step(Function *func, Float x, Float val_1, Float val_2)
    : func(func), x(x), val_1(val_1), val_2(val_2)
{
    bounds = new Bounds1Df(func->a, func->b);
}

Step::~Step() {}

Float Step::eval(Float sample) const
{
    return (sample < x) ? val_1 : val_2;
}

Float Step::eval(const std::vector<Float> &samples) const
{
    Float mean = 0.0;

    for (int i = 0; i < samples.size(); ++i)
    {
        mean += eval(samples[i]);
    }

    mean /= Float(samples.size());

    return mean;
}

Float Step::eval_integral(Float sample) const
{
    return func->eval(sample) * (func->b - func->a);
}

Float Step::eval_integral(const std::vector<Float> &samples) const
{
    Float sum = 0.0;

    for (int i = 0; i < samples.size(); ++i)
    {
        sum += eval_integral(samples[i]);
    }

    return sum / Float(samples.size());
}

Float Step::eval_function(Float sample) const
{
    return func->eval(sample);
}

Float Step::expected() const
{
    return eval(func->integrate());
}

Float Step::expected_integral() const
{
    return func->integrate();
}