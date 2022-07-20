#pragma once

#include <conver/conver.h>
#include <conver/problem.h>

template <typename T>
struct PRatioExp : Technique<T>
{
    PRatioExp(Float maj) : maj(maj)
    {
        // blah
    }

    virtual T eval_problem(const Problem<T> *problem, int samples, int &cost, pcg32 &rng) const
    {
        Float tau = maj / problem->getBounds()->uniformPDF();
        Float sample = rng.nextDouble();
        Float val = exp(-tau);
        Float cdf = val;
        Float prob = val;
        Float Tr = Float(1.0);
        Float i = 1.0;

        while (cdf < sample)
        {
            Float x = problem->getBounds()->uniformSample(rng);
            Float dense = problem->eval_integral(x);
            Tr *= (tau - dense) / tau;
            prob *= tau / i;
            cost++;

            cdf += prob;
            i += 1.0;
        }

        return Tr;
    }

    virtual T sample_problem(const Problem<T> *problem, int samples, int &cost, pcg32 &rng) const
    {
        // What does this really have to do with anything
        throw new ShouldNotEvaluateException("sample problem blanchet");

        return (0.0);
    }

    virtual void setMaj(T val) { maj = val; }

    virtual std::string getName() const { return "pratio_exp"; }

    Float maj;
};