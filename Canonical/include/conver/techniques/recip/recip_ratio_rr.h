#pragma once

#include <conver/conver.h>
#include <conver/problem.h>

// TODO: delete this implementation
template <typename T>
struct RatioRRRecip : Technique<T>
{
    // this takes as input little g, and computes big G in eval_problem
    RatioRRRecip(Float maj) : maj(maj)
    {
        // blah
    }

    virtual T eval_problem(const Problem<T> *problem, int samples, int &cost, pcg32 &rng) const
    {
        // this estimator was a bad idea
        Float pdf = problem->getBounds()->uniformPDF();
        // tau must be greater than 1.0
        Float tau = std::max(maj / pdf, 2.0); // (maj * (b - a))

        Float weight = 1.0 / tau;
        Float recip = 0.0;

        do
        {
            Float rr_samp = rng.nextDouble();

            if (rr_samp > (1.0 / tau))
            {
                // fail
                recip += weight / (tau * (1.0 - 1.0 / tau));
                break;
            }

            cost++;

            recip += weight;
            Float pos = problem->getBounds()->uniformSample(rng);
            weight *= (tau - problem->eval_integral(pos));
        } while (true);

        return recip;
    }

    virtual T sample_problem(const Problem<T> *problem, int samples, int &cost, pcg32 &rng) const
    {
        // What does this really have to do with anything
        throw new ShouldNotEvaluateException("sample problem " + getName());

        return (0.0);
    }

    virtual std::string getName() const { return "ratio_rr_recip"; }

    Float maj;
};