#pragma once

#include <conver/conver.h>
#include <conver/problem.h>

template <typename T>
struct Technique
{
public:
    virtual T eval_problem(const Problem<T> *problem, int samples, int &cost, pcg32 &rng) const = 0;
    virtual T sample_problem(const Problem<T> *problem, int samples, int &cost, pcg32 &rng) const = 0; // this might never be used

    virtual T eval_problem_secondary(const Problem<T> *problem, int samples, Float &cost, pcg32 &rng)
    {
        T val = 0.0;
        int tmp_cost = 0;

        for (int i = 0; i < samples; ++i)
        {
            val = val + 1.0 / Float(i + 1) * (eval_problem(problem, 1, tmp_cost, rng) - val);
        }

        cost += tmp_cost;

        return val;
    }

    virtual T eval_heat_problem_secondary(const Problem<T> *problem, int samples, Float &cost, pcg32 &rng)
    {
        T val = 0.0;
        int tmp_cost = 0;

        for (int i = 0; i < samples; ++i)
        {
            val = val + 1.0 / Float(i + 1) * (eval_problem(problem, 1, tmp_cost, rng) - val);
        }

        cost += Float(tmp_cost) / Float(samples);

        return val;
    }

    virtual T eval_integral_problem_secondary(Problem<T> *problem, int samples, Float &cost, pcg32 &rng)
    {
        T val = 0.0;
        int tmp_cost = 0;

        for (int i = 0; i < samples; ++i)
        {
            Float weight = problem->pre_sample(rng);
            val += weight * eval_problem(problem, 1, tmp_cost, rng);
        }

        cost += tmp_cost;
        val /= Float(samples);

        return val;
    }

    virtual void setMaj(T Maj) {}

    virtual std::string getName() const = 0;

protected:
};

template <typename T>
struct NaiveMC : Technique<T>
{
    virtual T eval_problem(const Problem<T> *problem, int samples, int &cost, pcg32 &rng) const
    {
        std::vector<Float> sampless = std::vector<Float>();

        for (int i = 0; i < samples; ++i)
        {
            sampless.push_back(problem->getBounds()->uniformSample(rng));
            cost++;
        }

        return problem->eval(problem->eval_integral(sampless));
    }

    // // since this is biased, samples is handled within eval_problem
    // virtual T eval_problem_secondary(const Problem<T> *problem, int samples, Float &cost, pcg32 &rng)
    // {
    //     int tmp_cost = 0;
    //     T eval = eval_problem(problem, samples, tmp_cost, rng);
    //     cost += tmp_cost;
    //     return eval;
    // }

    virtual T sample_problem(const Problem<T> *problem, int samples, int &cost, pcg32 &rng) const
    {
        // this does nothing for naive MC
        throw new ShouldNotEvaluateException("sample problem naive mc");

        return (0.0);
    }

    virtual std::string getName() const { return "naive_mc"; }
};

template <typename T>
struct Expected : Technique<T>
{
    virtual T eval_problem(const Problem<T> *problem, int samples, int &cost, pcg32 &rng) const
    {
        cost++;
        return problem->expected();
    }

    virtual T sample_problem(const Problem<T> *problem, int samples, int &cost, pcg32 &rng) const
    {
        // this does nothing for naive MC
        throw new ShouldNotEvaluateException("sample problem naive mc");

        return (0.0);
    }

    virtual std::string getName() const { return "expected"; }
};

template <typename T>
struct TestMC : Technique<T>
{
    virtual T eval_problem(const Problem<T> *problem, int samples, int &cost, pcg32 &rng) const
    {
        std::vector<Float> sampless = std::vector<Float>();

        for (int i = 0; i < samples; ++i)
        {
            sampless.push_back(problem->getBounds()->uniformSample(rng));
            cost++;
        }

        return problem->eval(sampless);
    }

    // // since this is biased, samples is handled within eval_problem
    // virtual T eval_problem_secondary(const Problem<T> *problem, int samples, Float &cost, pcg32 &rng)
    // {
    //     int tmp_cost = 0;
    //     T eval = eval_problem(problem, samples, tmp_cost, rng);
    //     cost += tmp_cost;
    //     return eval;
    // }

    virtual T sample_problem(const Problem<T> *problem, int samples, int &cost, pcg32 &rng) const
    {
        // this does nothing for naive MC
        throw new ShouldNotEvaluateException("sample problem naive mc");

        return (0.0);
    }

    virtual std::string getName() const { return "mc"; }
};

template <typename T>
struct NaiveFD : Technique<T>
{
    virtual T eval_problem(const Problem<T> *problem, int samples, int &cost, pcg32 &rng) const
    {
        T step_size = 1.5 / Float(samples);
        // TODO: how to incorporate cost here
        return problem->eval(step_size);
    }

    virtual T sample_problem(const Problem<T> *problem, int samples, int &cost, pcg32 &rng) const
    {
        // this does nothing for naive FD
        throw new ShouldNotEvaluateException("sample problem naive fd");

        return (0.0);
    }

    virtual std::string getName() const { return "naive_fd"; }
};

// template <typename T>
struct UnbiasedNaiveFD : Technique<Float>
{
    virtual Float eval_problem(const Problem<Float> *problem, int samples, int &cost, pcg32 &rng) const
    {
        // TODO: figure out what these original techniques were supposed to be used for
        // this assumes problem->eval evaluates a finite difference
        Float mean = 0.0;

        for (int i = 0; i < samples; ++i)
        {
            Float val = rng.nextDouble();

            Float r = 0.5;
            Float blanchet_pmf = r;
            Float cmf = blanchet_pmf;
            int blanchet_n = 0;

            while (cmf < val)
            {
                ++blanchet_n;
                blanchet_pmf *= 1.0 - r;
                cmf += blanchet_pmf;
            }

            Float base_eval = problem->eval(1.5);
            Float p1_eval = problem->eval(1.5 * pow(2.0, -Float(blanchet_n + 1)));
            Float p_eval = problem->eval(1.5 * pow(2.0, -Float(blanchet_n)));

            mean += base_eval + (p1_eval - p_eval) / blanchet_pmf;
        }

        mean /= Float(samples);

        return mean;
    }

    virtual Float sample_problem(const Problem<Float> *problem, int samples, int &cost, pcg32 &rng) const
    {
        // this does nothing for naive FD
        throw new ShouldNotEvaluateException("sample problem naive fd");

        return (0.0);
    }

    virtual std::string getName() const { return "unbiased_naive_fd"; }
};