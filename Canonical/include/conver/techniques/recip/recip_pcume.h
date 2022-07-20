#pragma once

#include <conver/conver.h>
#include <conver/problem.h>

template <typename T>
struct PCumeRecip : Technique<T>
{
    PCumeRecip(Float maj) : maj(maj)
    {
        // blah
    }

    virtual T eval_problem(const Problem<T> *problem, int samples, int &cost, pcg32 &rng) const
    {
        Float pdf = problem->getBounds()->uniformPDF();
        Float tau = maj / pdf;

        Float weight = 1.0;
        Float sum = 0.0;

        while (true)
        {
            Float rr = rng.nextDouble();
            sum += weight;
            Float pos = problem->getBounds()->uniformSample(rng);
            cost++;

            weight *= (tau - problem->eval_integral(pos)) / tau;
            Float prob = std::min(1.0, weight);
            if (rr > prob)
            {
                break;
            }
            weight /= prob;
        }

        return sum * 1.0 / tau;
    }

    virtual T sample_problem(const Problem<T> *problem, int samples, int &cost, pcg32 &rng) const
    {
        // What does this really have to do with anything
        throw new ShouldNotEvaluateException("sample problem " + getName());

        return (0.0);
    }

    virtual void setMaj(T val) { maj = val; }

    virtual std::string getName() const { return "pcume_recip"; }

    Float maj;
};