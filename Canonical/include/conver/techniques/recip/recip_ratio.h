#pragma once

#include <conver/conver.h>
#include <conver/problem.h>

template <typename T>
struct PRatioRecip : Technique<T>
{
    PRatioRecip(Float maj) : maj(maj)
    {
        // blah
    }

    virtual T eval_problem(const Problem<T> *problem, int samples, int &cost, pcg32 &rng) const
    {
        Float pdf = problem->getBounds()->uniformPDF();
        Float tau = maj / pdf;

        Float p = (1.0 - 1.0 / tau);
        int k = std::ceil(log(1.0 - rng.nextDouble()) / log(1.0 - p)) - 1;

        Float prod = 1.0;
        for (int i = 0; i < k; ++i)
        {
            cost++;
            Float pos = problem->getBounds()->uniformSample(rng);
            prod *= (tau - problem->eval_integral(pos));
        }

        return prod / (tau - 1.0);
    }

    virtual T sample_problem(const Problem<T> *problem, int samples, int &cost, pcg32 &rng) const
    {
        // What does this really have to do with anything
        throw new ShouldNotEvaluateException("sample problem blanchet");

        return (0.0);
    }

    virtual std::string getName() const { return "pratio_recip"; }

    Float maj;
};