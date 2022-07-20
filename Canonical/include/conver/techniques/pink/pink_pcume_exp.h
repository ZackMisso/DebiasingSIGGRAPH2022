#pragma once

#include <conver/conver.h>
#include <conver/problem.h>
#include <random>

template <typename T>
struct PCumePinkExp : Technique<T>
{
    PCumePinkExp(Float gl_maj, Float c) : gl_maj(gl_maj), c(c)
    {
        // blah
    }

    virtual T eval_problem(const Problem<T> *problem, int samples, int &cost, pcg32 &rng) const
    {
        std::default_random_engine generator;
        generator.seed(uint32_t(rng.nextDouble() * 0xFFFFFFFFFFFF)); // todo make nicer
        std::gamma_distribution<Float> distribution(1.0 / (c * c), c * c);

        Float mult = distribution(generator);

        Float maj = gl_maj * mult;
        Float pdf = problem->getBounds()->uniformPDF();
        Float tau = maj / pdf;

        Float weight = 1.0;
        Float sum = 0.0;
        Float i = 1.0;

        while (true)
        {
            Float rr = rng.nextDouble();
            sum += weight;
            Float pos = problem->getBounds()->uniformSample(rng);
            cost++;

            Float eval = problem->eval_integral(pos) * mult;
            weight *= (1.0 / i) * (tau - eval);
            Float prob = std::min(1.0, weight);
            if (rr > prob)
            {
                break;
            }
            weight /= prob;
            i += 1.0;
        }

        return sum * std::exp(-tau);
    }

    virtual T sample_problem(const Problem<T> *problem, int samples, int &cost, pcg32 &rng) const
    {
        // What does this really have to do with anything
        throw new ShouldNotEvaluateException("sample problem " + getName());

        return (0.0);
    }

    virtual void setMaj(T val) { gl_maj = val; }

    virtual std::string getName() const { return "pcume_pink_exp"; }

    Float gl_maj;
    Float c;
};