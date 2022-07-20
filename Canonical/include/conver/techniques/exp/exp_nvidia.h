#pragma once

#include <conver/conver.h>
#include <conver/problem.h>

template <typename T>
struct NvidiaExp : Technique<T>
{
    NvidiaExp(Float maj) : maj(maj)
    {
        // blah
    }

    virtual T eval_problem(const Problem<T> *problem, int samples, int &cost, pcg32 &rng) const
    {
        double p_z = 0.9;
        double c = 2.0;
        double l = problem->getBounds()->getExtents();

        int m = determine_tuple_size(maj, p_z, c);

        int k = 0; // this is N
        std::vector<double> rr_weights = std::vector<double>();

        aggressive_russian_roulette(rng, p_z, rr_weights, k);
        // TODO - maybe do optional density rotations

        std::vector<double> X_i = std::vector<double>();

        for (int i = 0; i <= k; ++i)
        {
            double u_i = rng.nextDouble();

            double x_i = 0.0;

            for (int j = 0; j < m; ++j)
            {
                double loc = l / double(m) * (u_i + j);
                x_i += problem->eval_function(loc);
                cost++;
            }

            X_i.push_back(-x_i * l / double(m));
        }

        double trans = 0.0;

        for (int i = 0; i <= k; ++i)
        {
            std::vector<double> means = std::vector<double>();
            std::vector<double> pivot = std::vector<double>();

            pivot_array(X_i, pivot, maj, i);
            elementry_means(pivot, means);

            double X_j = X_i[i];
            double sum = 0.0;
            double k_fact = 1.0;

            for (int j = 0; j <= k; ++j)
            {
                if (j != 0)
                    k_fact /= double(j);
                sum += means[j] * k_fact * rr_weights[j];
            }

            trans += 1.0 / double(k + 1) * exp(X_j) * sum;
        }

        // if (std::isnan(trans))
        // {
        //     std::cout << "FUCK" << std::endl;
        //     std::cout << "k: " << k << std::endl;
        // }

        return trans;
    }

    virtual T sample_problem(const Problem<T> *problem, int samples, int &cost, pcg32 &rng) const
    {
        // What does this really have to do with anything
        throw new ShouldNotEvaluateException("sample problem " + getName());

        return (0.0);
    }

    // Need to relearn what this method is supposed to do
    //
    // Exponential form:
    // Computes the expected value of the Bhanot and Kennedy estimator
    // E[N_BK] = K + (K! / c^K)(e^c - sum^K c^k/k!)
    //
    // Reciprocal form:
    // Computes the expected value of a modified Bhanot and Kennedy estimator
    // E[N_BK] = K + maj / c
    //
    double expected_order(double c, double p_z) const
    {
        double K = floor(c);
        double t = 1.0;
        double sum = 1.0;

        for (int k = 1; k <= K; ++k)
        {
            t = t * c / double(k);
            sum += t;
        }

        double E_n = K + (exp(c) - sum) / t;

        return (1.0 - p_z) * E_n;
    }

    // this determintes the tuple size to make this estimator have the same cost as
    // the pseries-cmf estimator
    int determine_tuple_size(double maj, double p_z, double c) const
    {
        double cmf_n = ncmf(maj);
        double bk_n = expected_order(c, p_z);

        return fmax(1, floor(cmf_n / (bk_n + 1.0) + 0.5));
    }

    int ncmf(double maj) const
    {
        return ceil(pow((0.015 + maj) * (0.65 + maj) * (60.3 + maj), 1.0 / 3.0));
    }

    void aggressive_russian_roulette(
        pcg32 &rng,
        double p_z,
        std::vector<double> &weights,
        int &k) const
    {
        double w0 = 1.0;
        // initial probability for continuing
        double P = 1.0 - p_z;
        double u = 1.0 - rng.nextDouble();

        weights.push_back(w0);

        if (P <= u)
        {
            k = 0;
            return;
        }

        k = 2;
        int K = 2;
        double c = 2;

        for (int i = 0; i < K; ++i)
        {
            weights.push_back(1.0 / P);
        }

        double i = K + 1.0;
        while (true)
        {
            double c_i = fmin(c / i, 1.0);
            P *= c_i;

            // if (i > 100)
            // {
            //     std::cout << "i: " << i << std::endl;
            //     std::cout << "P: " << 1.0 / P << std::endl;
            //     std::cout << "u: " << u << std::endl;
            // }

            if (P <= u)
            {
                k = int(i - 1.0);
                return;
            }
            i += 1.0;

            weights.push_back(1.0 / P);
        }
    }

    void pivot_array(const std::vector<double> &samples,
                     std::vector<double> &pivoted,
                     double maj,
                     int j) const
    {
        pivoted.clear();

        double x_j = samples[j];

        for (int i = 0; i < samples.size(); ++i)
        {
            if (i != j)
            {
                pivoted.push_back(samples[i] - x_j);
            }
        }
    }

    void elementry_means(const std::vector<double> &samples,
                         std::vector<double> &means) const
    {
        means.push_back(1.0);
        for (int i = 0; i < samples.size(); ++i)
            means.push_back(0.0);

        for (int i = 0; i < samples.size(); ++i)
        {
            for (int k = i; k > 0; k--)
            {
                means[k] = means[k] + double(k) / double(samples.size()) *
                                          (means[k - 1] * samples[i] - means[k]);
            }
        }
    }

    virtual void setMaj(T val) { maj = val; }

    virtual std::string getName() const { return "nvidia_exp"; }

    Float maj;
};
