#pragma once

#include <conver/conver.h>
#include <conver/problem.h>

template <typename T>
struct SaiTele : Technique<T>
{
    SaiTele(Float r = 1.0 / 8.0) : r(r) {}

    virtual T eval_problem(const Problem<T> *problem, int samples, int &cost, pcg32 &rng) const
    {
        // choose n and compute the blanchet pmf
        Float val = rng.nextDouble();
        Float pmf = r;
        Float cmf = 0.0;
        int n = 0;

        Float pos = problem->getBounds()->uniformSample(rng);
        Float zim1 = problem->eval_integral(pos);
        Float sum = problem->eval(zim1);
        cost++;

        do
        {
            cmf += pmf;

            n++;

            pos = problem->getBounds()->uniformSample(rng);
            Float int_eval_i = problem->eval_integral(pos);
            cost++;
            Float zi = zim1 * (Float(n) / Float(n + 1)) + int_eval_i * (1.0 / Float(n + 1));
            // Float zi = zim1 + int_eval_i;
            Float ti = problem->eval(zi) - problem->eval(zim1);
            sum += ti / (1.0 - cmf);
            zim1 = zi;
            pmf *= 1.0 - r;
        } while (cmf < val);

        return sum;
    }

    virtual T sample_problem(const Problem<T> *problem, int samples, int &cost, pcg32 &rng) const
    {
        // What does this really have to do with anything
        throw new ShouldNotEvaluateException("sample problem blanchet");

        return (0.0);
    }

    virtual std::string getName() const { return "was_tele"; }

    Float r;
};