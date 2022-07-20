#pragma once

#include <conver/conver.h>
#include <conver/problem.h>

struct PhotonAlphaUni : Technique<Float>
{
    struct Photon
    {
        Photon(Float energy, Float dist)
            : energy(energy), dist(dist) {}

        Float energy;
        Float dist;
    };

    PhotonAlphaUni(Float alpha = 0.9,
                   Float c = 0.0,
                    Float base_r = 1.0,
                    int base_photons = 1000,
                    int base_samples = 128)
        : alpha(alpha),
        c(c),
          base_r(base_r),
          base_photons(base_photons),
          base_samples(base_samples) {}

    virtual Float eval_problem(const Problem<Float> *problem, int samples, int &cost, pcg32 &rng) const
    {
        Float eval_base = 0.0;
        Float eval_p1 = 0.0;
        Float eval_p = 0.0;

        Float ran = rng.nextDouble();

        Float n = std::pow(1.0 - ran, 1.0 / (alpha - 1.0));

        Float n_1 = std::floor(n);
        Float n_2 = std::ceil(n);

        n_2 = 2.0;
        while (n_2 < n) n_2 *= 2.0;
        n_1 = n_2 / 2.0;

        Float r = base_r * std::sqrt(std::pow(n_1, alpha - 1.0));
        Float r1 = base_r * std::sqrt(std::pow(n_2, alpha - 1.0));

        Float alpha_pmf = std::pow(n_1, alpha - 1.0) - std::pow(n_2, alpha - 1.0);

        int total_photons = base_photons;
        if (c > 0.0)
            total_photons *= std::pow(n_2, 1.0 - c * alpha);

        for (int l = 0; l < base_samples; ++l)
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

        return eval_base + (eval_p1 - eval_p) / alpha_pmf;
    }

    virtual Float sample_problem(const Problem<Float> *problem, int samples, int &cost, pcg32 &rng) const
    {
        // What does this really have to do with anything
        throw new ShouldNotEvaluateException("sample problem blanchet final");

        return (0.0);
    }

    virtual std::string getName() const { return "photon_alpha_uni"; }

    Float base_r;
    Float alpha;
    Float c;
    int base_photons;
    int base_samples;
};