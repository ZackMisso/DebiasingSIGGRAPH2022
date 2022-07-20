#pragma once

#include <conver/conver.h>
#include <conver/problem.h>

template <typename T>
struct PResRatioRecip : Technique<T>
{
    PResRatioRecip(Float maj, Float min) : maj(maj), min(min)
    {
        // blah
    }

    virtual T eval_problem(const Problem<T> *problem, int samples, int &cost, pcg32 &rng) const
    {
        Float pdf = problem->getBounds()->uniformPDF();
        Float tau_max = maj / pdf;
        Float tau_min = min / pdf;

        Float p = 1.0 - (tau_max - tau_min) / (tau_max);
        int k = std::ceil(log(1.0 - rng.nextDouble()) / log(1.0 - p)) - 1;

        Float mult = 1.0;
        for (int i = 0; i < k; ++i)
        {
            cost++;
            Float pos = problem->getBounds()->uniformSample(rng);
            Float num = (tau_max - problem->eval_integral(pos));
            Float den = (tau_max - tau_min);
            mult *= num / den;
        }

        return mult * (1.0 / tau_min);
    }

    virtual T sample_problem(const Problem<T> *problem, int samples, int &cost, pcg32 &rng) const
    {
        // What does this really have to do with anything
        throw new ShouldNotEvaluateException("sample problem blanchet");

        return (0.0);
    }

    virtual void setMaj(T val)
    {
        maj = val;
        if (maj < min)
            min = maj / 4.0;
    }

    virtual std::string getName() const { return "res_ratio_recip"; }

    Float maj;
    Float min;
};