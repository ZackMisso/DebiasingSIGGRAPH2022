#pragma once

#include <conver/conver.h>
#include <conver/problem.h>

template <typename T>
struct BinBernoulli : Technique<T>
{
    BinBernoulli(Float r = 0.65) : blanchet_r(r) {}

    virtual T eval_problem(const Problem<T> *problem, int samples, int &cost, pcg32 &rng) const
    {
        // base number of integral evaluations is 1, debias from there
        Float count = 1.0;
        while (true)
        {
            cost++;
            Float pos = problem->getBounds()->uniformSample(rng);
            if (problem->eval_integral(pos) > 0.5)
                break;
            count += 1.0;
        }
        return count;
    }

    virtual T sample_problem(const Problem<T> *problem, int samples, int &cost, pcg32 &rng) const
    {
        // What does this really have to do with anything
        throw new ShouldNotEvaluateException("sample problem bernoulli");

        return (0.0);
    }

    virtual std::string getName() const { return "bernoulli"; }

    Float blanchet_r;
};