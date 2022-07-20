#pragma once

#include <conver/conver.h>
#include <conver/problem.h>

template <typename T>
struct BinNonBinBernoulli : Technique<T>
{
    BinNonBinBernoulli(Float maj = 1.0) : maj(maj) {}

    virtual T eval_problem(const Problem<T> *problem, int samples, int &cost, pcg32 &rng) const
    {
        // base number of integral evaluations is 1, debias from there
        Float pdf = problem->getBounds()->uniformPDF();
        Float tau = maj / pdf;
        Float count = 0.0;

        while (true)
        {
            cost++;
            Float pos = problem->getBounds()->uniformSample(rng);
            Float eval = problem->eval_integral(pos);
            Float rr = (tau - eval) / tau;
            count += 1.0;

            if (rng.nextDouble() > rr)
                break;
        }
        return 1.0 / (tau) * (count);
    }

    virtual T sample_problem(const Problem<T> *problem, int samples, int &cost, pcg32 &rng) const
    {
        // What does this really have to do with anything
        throw new ShouldNotEvaluateException("sample problem bernoulli");

        return (0.0);
    }

    virtual void setMaj(T val) { maj = val; }

    virtual std::string getName() const { return "bernoulli_nonbin"; }

    Float maj;
};