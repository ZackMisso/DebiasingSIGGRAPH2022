#pragma once

#include <conver/conver.h>
#include <conver/problem.h>

template <typename T>
struct PCumePink : Technique<T>
{
    // c needs to be the same as the one passed to the problem.
    // TODO: maybe setup some way of getting arbitrary parameters from
    //       problems without having to store the parameter in two separate
    //       locations
    PCumePink(Float maj, Float c) : maj(maj), c(c)
    {
        // blah
    }

    virtual T eval_problem(const Problem<T> *problem, int samples, int &cost, pcg32 &rng) const
    {
        Float pdf = problem->getBounds()->uniformPDF();
        Float tau = maj / pdf;

        Float weight = 1.0;
        Float sum = 0.0;
        Float i = 1.0;

        Float prod_weight = 1.0;
        Float inv_factor = 1.0 / (1.0 + tau * c * c);

        while (true)
        {
            Float rr = rng.nextDouble();
            sum += weight;
            Float pos = problem->getBounds()->uniformSample(rng);
            cost++;

            Float tau_eval = problem->eval_integral(pos);
            prod_weight = (1.0 + (i - 1.0) * c * c) * inv_factor;

            weight *= (1.0 / i) * prod_weight * (tau - tau_eval);
            Float prob = std::min(1.0, weight);
            if (rr > prob)
            {
                break;
            }
            // if (i > 100.0)
            //   std::cout << "weight: " << weight << std::endl;
            weight /= prob;
            i += 1.0;
        }

        return sum * std::pow(1.0 + tau * c * c, -1.0 / (c * c));
    }

    virtual T sample_problem(const Problem<T> *problem, int samples, int &cost, pcg32 &rng) const
    {
        // What does this really have to do with anything
        throw new ShouldNotEvaluateException("sample problem " + getName());

        return (0.0);
    }

    virtual void setMaj(T val) { maj = val; }

    virtual std::string getName() const { return "pcume_pink"; }

    Float maj;
    Float c;
};
