#pragma once

#include <conver/conver.h>
#include <conver/problem.h>

template <typename T>
struct PCmfRecip : Technique<T>
{
    PCmfRecip(Float maj, Float min) : maj(maj), min(min)
    {
        // blah
    }

    virtual T eval_problem(const Problem<T> *problem, int samples, int &cost, pcg32 &rng) const
    {
        Float pdf = problem->getBounds()->uniformPDF();
        Float tau_max = maj / pdf;
        Float tau_min = min / pdf;

        Float weight = 1.0;
        Float sum = 0.0;

        Float end_portion = 0.90;
        Float p = 1.0 - (tau_max - tau_min) / (tau_max);
        int free_k = std::ceil(log(1.0 - end_portion) / log(1.0 - p)) - 1;
        if (free_k < 0) // this can happen apparently
            free_k = 0;
        free_k = std::min(100, free_k);

        // compute the first few iterations free of a pmf
        for (int i = 0; i < free_k; ++i)
        {
            sum += weight;
            Float pos = problem->getBounds()->uniformSample(rng);
            cost++;
            weight *= (tau_max - problem->eval_integral(pos)) / tau_max;
        }

        int index = free_k;
        while (true)
        {
            Float rr = rng.nextDouble();
            sum += weight;
            Float pos = problem->getBounds()->uniformSample(rng);
            cost++;

            weight *= (tau_max - problem->eval_integral(pos)) / tau_max;
            Float prob = std::min(1.0, weight);
            if (rr > prob || index > 1000) // what
            {
                break;
            }
            weight /= prob;
            index++;
        }

        return sum * 1.0 / tau_max;
    }

    virtual T sample_problem(const Problem<T> *problem, int samples, int &cost, pcg32 &rng) const
    {
        // What does this really have to do with anything
        throw new ShouldNotEvaluateException("sample problem " + getName());

        return (0.0);
    }

    virtual void setMaj(T val)
    {
        maj = val;
        if (maj < min)
            min = maj / 4.0;
    }

    virtual std::string getName() const { return "pcmf_recip"; }

    Float maj;
    Float min;
};