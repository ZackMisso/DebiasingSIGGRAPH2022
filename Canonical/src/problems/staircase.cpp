#include <conver/problem.h>

Staircase::Staircase(Function *func, Float x_step, Float y_step, Float x_start, Float y_start)
    : func(func), x_step(x_step), y_step(y_step), x_start(x_start), y_start(y_start)
{
    bounds = new Bounds1Df(func->a, func->b);
}

Staircase::~Staircase() {}

Float Staircase::eval(Float sample) const
{
    // this assumes x_start is less than x_step
    Float first_step = x_step - x_start;
    if (sample < first_step)
        return y_start;
    sample -= first_step;
    Float ret = y_start - y_step;
    while (sample < x_step)
    {
        ret -= y_step;
        sample -= x_step;
    }
    return (ret > 0.0) ? ret : 0.0;
}

Float Staircase::eval(const std::vector<Float> &samples) const
{
    Float mean = 0.0;

    for (int i = 0; i < samples.size(); ++i)
    {
        mean += eval(samples[i]);
    }

    mean /= Float(samples.size());

    return mean;
}

Float Staircase::eval_integral(Float sample) const
{
    return func->eval(sample) * (func->b - func->a);
}

Float Staircase::eval_integral(const std::vector<Float> &samples) const
{
    Float sum = 0.0;

    for (int i = 0; i < samples.size(); ++i)
    {
        sum += eval_integral(samples[i]);
    }

    return sum / Float(samples.size());
}

Float Staircase::eval_function(Float sample) const
{
    return func->eval(sample);
}

Float Staircase::expected() const
{
    return eval(func->integrate());
}

Float Staircase::expected_integral() const
{
    return func->integrate();
}