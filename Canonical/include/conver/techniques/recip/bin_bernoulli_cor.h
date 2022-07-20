#pragma once

#include <conver/conver.h>
#include <conver/problem.h>

template <typename T>
struct BinBernoulliCor : Technique<T>
{
    BinBernoulliCor(Float r = 0.65) : blanchet_r(r) {}

    virtual T eval_problem(const Problem<T> *problem, int samples, int &cost, pcg32 &rng) const
    {
        // base number of integral evaluations is 1, debias from there
        Float count = 1.0;
        Float offset = rng.nextDouble();
        Float denom = 2.0;
        Float num = 1.0;
        bool exit = false;

        while (!exit)
        {
            while (num < denom && !exit)
            {
                cost++;
                Float pos = offset + num / denom;
                if (pos > 1.0)
                    pos -= 1.0;
                if (problem->eval_integral(pos) > 0.5)
                    exit = true;
                else
                    count += 1.0;
                num += 2.0;
            }

            num = 1.0;
            denom *= 2.0;
        }
        return count;
    }

    virtual T sample_problem(const Problem<T> *problem, int samples, int &cost, pcg32 &rng) const
    {
        // What does this really have to do with anything
        throw new ShouldNotEvaluateException("sample problem bernoulli cor");

        return (0.0);
    }

    virtual std::string getName() const { return "bernoulli_cor"; }

    Float blanchet_r;
};