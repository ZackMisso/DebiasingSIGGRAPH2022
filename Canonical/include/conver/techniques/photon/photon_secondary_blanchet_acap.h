#pragma once

#include <conver/conver.h>
#include <conver/problem.h>

struct BlanchetSecondaryACAP : Technique<Float>
{
    struct Photon
    {
        Photon(Float energy, Float dist)
            : energy(energy), dist(dist) {}

        Float energy;
        Float dist;
    };

    BlanchetSecondaryACAP(Float r = 0.65, int base_photons = 1000, int samples = 128)
        : blanchet_r(r), base_photons(base_photons), samples(samples) {}

    virtual Float eval_problem(const Problem<Float> *problem, int samples, int &cost, pcg32 &rng) const
    {
        // const PhotonProblem *prob = (const PhotonProblem *)problem;

        Float eval_base = 0.0;
        Float eval_p1 = 0.0;
        Float eval_p = 0.0;

        Float normzation = 10.5844;

        Float rr;
        do
        {
            rr = rng.nextDouble();
        } while (rr == 0.0);

        Float running_cmf = 0.0;
        Float pmf;
        int n = 0;
        while (running_cmf < rr)
        {
            ++n;
            pmf = std::pow(Float(n), -1.1) / normzation;
            running_cmf += pmf;
        }

        Float blanchet_pmf = pmf;

        Float base_r = 1.0;

        Float r = base_r / std::pow(Float(n), -1.1);
        Float r1 = base_r / std::pow(Float(n + 1), -1.1);

        int total_photons = base_photons * std::pow(Float(n), 0.05);

        for (int l = 0; l < samples; ++l)
        {
            std::vector<Float> base_samples = std::vector<Float>();
            std::vector<Float> p_1_samples = std::vector<Float>();
            std::vector<Float> p_2_samples = std::vector<Float>();
            std::vector<Float> p1_samples = std::vector<Float>();

            Float base = 0.0;
            Float p = 0.0;
            Float p1 = 0.0;

            for (int i = 0; i < total_photons; ++i)
            {
                cost++;
                // sample direction
                Float cosTheta = 1.0 - rng.nextDouble();
                Float sinTheta = std::sqrt(std::max((Float)0, 1 - cosTheta * cosTheta));

                Float sinPhi, cosPhi;
                feign_sincos(2.0 * M_PI * rng.nextDouble(), &sinPhi, &cosPhi);

                Float xdir = Float(cosPhi) * sinTheta;
                Float ydir = Float(sinPhi) * sinTheta;
                Float zdir = cosTheta;

                if (zdir == 0.0)
                    continue;

                Float pdf = 1.0 / (2.0 * M_PI);

                Float dist_to_ground = -problem->get_variable(PHOTON_EMITTER_Z) / zdir;

                Float xloc = xdir * dist_to_ground;
                Float yloc = ydir * dist_to_ground;

                Float dist = std::sqrt(xloc * xloc + yloc * yloc);
                // why is the pdf not included here? What the heck?
                Float energy = problem->get_variable(PHOTON_EMITTER_POWER) / Float(total_photons);

                if (dist <= base_r)
                    base += energy;
                if (dist <= r)
                    p += energy;
                if (dist <= r1)
                    p1 += energy;
            }

            base /= M_PI * base_r * base_r;
            p /= M_PI * r * r;
            p1 /= M_PI * r1 * r1;

            eval_base = eval_base + 1.0 / Float(l + 1) * (base - eval_base);
            eval_p1 = eval_p1 + 1.0 / Float(l + 1) * (p1 - eval_p1);
            eval_p = eval_p + 1.0 / Float(l + 1) * (p - eval_p);
        }

        return eval_base + (eval_p1 - eval_p) / blanchet_pmf;
    }

    virtual Float sample_problem(const Problem<Float> *problem, int samples, int &cost, pcg32 &rng) const
    {
        // What does this really have to do with anything
        throw new ShouldNotEvaluateException("sample problem blanchet final");

        return (0.0);
    }

    virtual std::string getName() const { return "blanchet_acap"; }

    Float blanchet_r;
    int base_photons;
    int samples;
};