#include <conver/problem.h>

// TODO: fix all of this

PinkDavis::PinkDavis(Function *func, Float c) : optical_depth(func), c(c)
{
    bounds = new Bounds1Df(optical_depth->a, optical_depth->b);
}

PinkDavis::~PinkDavis() {}

Float PinkDavis::eval(Float sample) const
{
    return std::pow(1.0 + c * c * sample, -1.0 / (c * c));
}

Float PinkDavis::eval(const std::vector<Float> &samples) const
{
    Float mean = 0.0;

    for (int i = 0; i < samples.size(); ++i)
    {
        mean += eval(samples[i]);
    }

    mean /= Float(samples.size());

    return mean;
}

Float PinkDavis::eval_integral(Float sample) const
{
    return optical_depth->eval(sample) * (optical_depth->b - optical_depth->a);
}

Float PinkDavis::eval_integral(const std::vector<Float> &samples) const
{
    Float sum = 0.0;

    for (int i = 0; i < samples.size(); ++i)
    {
        sum += eval_integral(samples[i]);
    }

    return sum / Float(samples.size());
}

Float PinkDavis::eval_function(Float sample) const
{
    return optical_depth->eval(sample);
}

Float PinkDavis::expected() const
{
    return std::pow(1.0 + optical_depth->integrate() * c * c, -1.0 / (c * c));
}

Float PinkDavis::expected_integral() const
{
    return optical_depth->integrate();
}