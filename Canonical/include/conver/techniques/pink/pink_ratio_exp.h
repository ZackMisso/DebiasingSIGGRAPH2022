#pragma once

#include <conver/conver.h>
#include <conver/problem.h>
#include <random>

template <typename T>
struct RatioPink : Technique<T>
{
    // c needs to be the same as the one passed to the problem.
    // TODO: maybe setup some way of getting arbitrary parameters from
    //       problems without having to store the parameter in two separate
    //       locations
    RatioPink(Float maj, Float c) : maj(maj), c(c)
    {
        // blah
    }

    virtual T eval_problem(const Problem<T> *problem, int samples, int &cost, pcg32 &rng) const
    {

        std::default_random_engine generator;
        generator.seed(uint32_t(rng.nextDouble() * 0xFFFFFFFFFFFF)); // todo make nicer
        std::gamma_distribution<Float> distribution(1.0 / (c * c), c * c);

        Float mult = distribution(generator);

        Float scaled_maj = maj * mult;
        Float pdf = problem->getBounds()->uniformPDF();
        Float tau = scaled_maj / pdf;

        Float Tr = 1.0;
        Float t = problem->getBounds()->getStart();
        Float b = problem->getBounds()->getEnd();

        while (true)
        {
            t -= std::log(1.0 - rng.nextDouble()) / scaled_maj;

            if (t >= b)
                break;

            Float eval = problem->eval_integral(t) * mult;
            cost++;

            Tr *= (tau - eval) / tau;
        }

        return Tr;
    }

    virtual T sample_problem(const Problem<T> *problem, int samples, int &cost, pcg32 &rng) const
    {
        // What does this really have to do with anything
        throw new ShouldNotEvaluateException("sample problem " + getName());

        return (0.0);
    }

    virtual void setMaj(T val) { maj = val; }

    virtual std::string getName() const { return "ratio_pink"; }

    Float maj;
    Float c;
};
