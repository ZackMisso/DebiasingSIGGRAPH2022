#pragma once

#include <conver/conver.h>
#include <conver/problem.h>
#include <random>

template <typename T>
struct PCmfPinkExp : Technique<T>
{
    PCmfPinkExp(Float gl_maj, Float c) : gl_maj(gl_maj), c(c)
    {
        // blah
    }

    virtual T eval_problem(const Problem<T> *problem, int samples, int &cost, pcg32 &rng) const
    {
        std::default_random_engine generator;
        generator.seed(uint32_t(rng.nextDouble() * 0xFFFFFFFFFFFF)); // todo make nicer
        std::gamma_distribution<Float> distribution(1.0 / (c * c), c * c);

        Float mult = distribution(generator);

        Float maj = mult * gl_maj;
        Float pdf = problem->getBounds()->uniformPDF();
        Float tau_max = maj / pdf;

        Float weight = 1.0;
        Float sum = 0.0;

        Float end_portion = 0.99;
        Float last_pdf = exp(-tau_max);
        Float running_cdf = 0.0;
        Float rr;
        Float i = 1.0;
        Float Tr = 0.0;
        do
        {
            rr = rng.nextDouble();
        } while (rr == 0.0);
        while (running_cdf < end_portion)
        {
            Float pos = problem->getBounds()->uniformSample(rng);
            running_cdf += last_pdf;
            Float invI = 1.0 / i;
            Float dense = problem->eval_integral(pos) * mult;
            Float wi = invI * (tau_max - dense);
            last_pdf *= tau_max * invI;
            Tr += weight;
            weight *= wi;
            i += 1.0;
            cost++;
        }

        while (true)
        {
            Float rr = rng.nextDouble();
            Float accept = tau_max / i;
            if (accept <= rr)
                break;
            rr /= accept;

            Float pos = problem->getBounds()->uniformSample(rng);
            Float invI = 1.0 / i;
            Float dense = problem->eval_integral(pos) * mult;
            Float wi = invI * (tau_max - dense);

            weight *= (wi / accept);
            i += 1.0;
            cost++;
        }

        return Tr * exp(-tau_max);
    }

    virtual T sample_problem(const Problem<T> *problem, int samples, int &cost, pcg32 &rng) const
    {
        // What does this really have to do with anything
        throw new ShouldNotEvaluateException("sample problem " + getName());

        return (0.0);
    }

    virtual void setMaj(T val) { gl_maj = val; }

    virtual std::string getName() const { return "pcmf_pink_exp"; }

    Float gl_maj;
    Float c;
};