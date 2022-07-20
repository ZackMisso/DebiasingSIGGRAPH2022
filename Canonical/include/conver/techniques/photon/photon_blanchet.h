#pragma once

#include <conver/conver.h>
#include <conver/problem.h>

struct BlanchetPhoton : Technique<Float>
{
    struct Photon
    {
        Photon(Float energy, Float dist)
            : energy(energy), dist(dist) {}

        Float energy;
        Float dist;
    };

    BlanchetPhoton(Float r = 0.65, int base_photons = 100)
        : blanchet_r(r), base_photons(base_photons) {}

    virtual Float eval_problem(const Problem<Float> *problem, int samples, int &cost, pcg32 &rng) const
    {
        std::vector<Float> base_samples = std::vector<Float>();
        std::vector<Float> p_1_samples = std::vector<Float>();
        std::vector<Float> p_2_samples = std::vector<Float>();
        std::vector<Float> p1_samples = std::vector<Float>();

        // choose n and compute its pmf
        int n = std::ceil(log(1.0 - rng.nextDouble()) / log(1.0 - blanchet_r)) - 1;
        if (n < 0)
            n = 0; // this can happen apparently
        Float blanchet_pmf = blanchet_r * std::pow((1.0 - blanchet_r), n);
        n = 0; // debug

        // sample the n evals
        int total_n = pow(2, n + 1);
        int total_photons = total_n * base_photons / 2;
        total_photons = 1000;
        // const PhotonProblem *prob = (const PhotonProblem *)problem;
        // std::cout << "total photons: " << total_photons << std::endl;

        Float base_r = 1.0;
        Float r = base_r / Float(total_n / 2);
        Float r1 = r / 2.0;

        // std::cout << "r1: " << r1 << std::endl;
        // std::cout << "r: " << r << std::endl;
        // std::cout << std::endl;

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

        // return base;

        return base + (p1 - p) / blanchet_pmf;
        return base;
    }

    virtual Float sample_problem(const Problem<Float> *problem, int samples, int &cost, pcg32 &rng) const
    {
        // What does this really have to do with anything
        throw new ShouldNotEvaluateException("sample problem blanchet final");

        return (0.0);
    }

    virtual std::string getName() const { return "blanchet_photon"; }

    Float blanchet_r;
    int base_photons;
};