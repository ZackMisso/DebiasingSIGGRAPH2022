#include <conver/problem.h>

PowerLaw::PowerLaw(Function *func, Float alpha) : optical_depth(func), alpha(alpha)
{
    bounds = new Bounds1Df(optical_depth->a, optical_depth->b);
}

PowerLaw::~PowerLaw() {}

Float PowerLaw::eval(Float sample) const
{
    return 1.0 / std::pow(sample, alpha);
}

Float PowerLaw::eval(const std::vector<Float> &samples) const
{
    Float mean = 0.0;

    for (int i = 0; i < samples.size(); ++i)
    {
        mean += eval(samples[i]);
    }

    mean /= Float(samples.size());

    return mean;
}

Float PowerLaw::eval_integral(Float sample) const
{
    return optical_depth->eval(sample) * (optical_depth->b - optical_depth->a);
}

Float PowerLaw::eval_integral(const std::vector<Float> &samples) const
{
    Float sum = 0.0;

    for (int i = 0; i < samples.size(); ++i)
    {
        sum += eval_integral(samples[i]);
    }

    return sum / Float(samples.size());
}

Float PowerLaw::eval_function(Float sample) const
{
    return optical_depth->eval(sample);
}

Float PowerLaw::expected() const
{
    Float int_exp = optical_depth->integrate();
    return 1.0 / std::pow(int_exp, alpha);
}

Float PowerLaw::expected_integral() const
{
    return optical_depth->integrate();
}