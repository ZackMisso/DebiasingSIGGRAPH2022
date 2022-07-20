#include <conver/problem.h>

Reciprocal::Reciprocal(Function *func) : func(func)
{
    bounds = new Bounds1Df(func->a, func->b);
}

Reciprocal::~Reciprocal() {}

Float Reciprocal::eval(Float sample) const
{
    return 1.0 / sample;
}

Float Reciprocal::eval(const std::vector<Float> &samples) const
{
    Float mean = 0.0;

    for (int i = 0; i < samples.size(); ++i)
    {
        mean += eval(samples[i]);
    }

    mean /= Float(samples.size());

    return mean;
}

Float Reciprocal::eval(Float sample, Float min) const
{
    return 1.0 / std::max(sample, min);
}

Float Reciprocal::eval(const std::vector<Float> &samples, Float min) const
{
    Float mean = 0.0;

    for (int i = 0; i < samples.size(); ++i)
    {
        mean += eval(samples[i], min);
    }

    mean /= Float(samples.size());

    return mean;
}

Float Reciprocal::eval_rep_if_zero(Float sample, Float min) const
{
    if (sample < 0.00001)
        return 1.0 / min;
    else
        return 1.0 / sample;
}

Float Reciprocal::eval_rep_if_zero(const std::vector<Float> &samples, Float min) const
{
    Float mean = 0.0;

    for (int i = 0; i < samples.size(); ++i)
    {
        mean += eval_rep_if_zero(samples[i], min);
    }

    mean /= Float(samples.size());

    return mean;
}

Float Reciprocal::eval_rot(Float sample, Float epsilon, int m) const
{
    // broken
    Float fb = eval_function(func->b);
    Float fa = eval_function(func->a);

    Float mod_sample = sample + (func->b - func->a) / Float(m) * (0.5 - epsilon) * (fb - fa);
    return 1.0 / mod_sample;
}

Float Reciprocal::eval_rot(const std::vector<Float> &samples) const
{
    // broken
    Float mean = 0.0;

    // Float sum = eval_integral(samples);
    // sum += (func->b - func->a) * (0.5 - sample / (func->b - func->a)) * (fb - fa) / Float(samples.size());

    // return 1.0 / sum;

    return mean;
}

Float Reciprocal::eval_integral(Float sample) const
{
    return func->eval(sample) * (func->b - func->a);
}

Float Reciprocal::eval_function(Float sample) const
{
    return func->eval(sample);
}

Float Reciprocal::eval_integral(const std::vector<Float> &samples) const
{
    Float sum = 0.0;

    for (int i = 0; i < samples.size(); ++i)
    {
        sum += eval_integral(samples[i]);
    }

    return sum / Float(samples.size());
}

Float Reciprocal::expected() const
{
    return eval(func->integrate());
}

Float Reciprocal::expected_integral() const
{
    return func->integrate();
}