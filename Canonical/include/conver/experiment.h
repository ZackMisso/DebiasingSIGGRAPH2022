#pragma once

#include <conver/conver.h>
#include <conver/technique.h>

class Experiment
{
public:
    virtual ~Experiment() {}

    virtual void run() const = 0;
    virtual std::string getName() const = 0;
    virtual std::string getDataPath() const = 0;
    virtual void preProcess() = 0;

    virtual void run(int test) const
    {
        run();
    }
};

// class ReciprocalBinaryHeatExperiment : public Experiment
// {
// public:
//     virtual void run() const;
//     void run_experiment(pcg32 &rng,
//                         std::vector<Technique<Float> *> techniques,
//                         std::string prefix) const;
//     virtual std::string getName() const { return "bin_Heat_rand_func"; }
//     virtual std::string getDataPath() const
//     {
//         return "../results/" + getName() + "/";
//     }
//     virtual void preProcess()
//     {
//         std::string cmd = "mkdir " + getDataPath();
//         system(cmd.c_str());
//     }
// };

// class ReciprocalHeatPlotsScos : public Experiment
// {
// public:
//     virtual void run() const;
//     void run_experiment(pcg32 &rng,
//                         std::vector<Technique<Float> *> techniques,
//                         std::string prefix) const;
//     virtual std::string getName() const { return "recip_Heat"; }
//     virtual std::string getDataPath() const
//     {
//         return "../results/" + getName() + "/";
//     }
//     virtual void preProcess()
//     {
//         std::string cmd = "mkdir " + getDataPath();
//         system(cmd.c_str());
//     }
// };

// class ReciprocalHeatPlotsLin : public Experiment
// {
// public:
//     virtual void run() const;
//     void run_experiment(pcg32 &rng,
//                         std::vector<Technique<Float> *> techniques,
//                         std::string prefix) const;
//     virtual std::string getName() const { return "recip_Heat"; }
//     virtual std::string getDataPath() const
//     {
//         return "../results/" + getName() + "/";
//     }
//     virtual void preProcess()
//     {
//         std::string cmd = "mkdir " + getDataPath();
//         system(cmd.c_str());
//     }
// };

// class ReciprocalHeatPlotsRCos : public Experiment
// {
// public:
//     virtual void run() const;
//     void run_experiment(pcg32 &rng,
//                         std::vector<Technique<Float> *> techniques,
//                         std::string prefix) const;
//     virtual std::string getName() const { return "recip_Heat"; }
//     virtual std::string getDataPath() const
//     {
//         return "../results/" + getName() + "/";
//     }
//     virtual void preProcess()
//     {
//         std::string cmd = "mkdir " + getDataPath();
//         system(cmd.c_str());
//     }
// };

class ExponentialHeatPlots : public Experiment
{
public:
    virtual void run() const;
    virtual void run(int test) const;
    void run_experiment_noise(pcg32 &rng,
                              std::vector<Technique<Float> *> techniques,
                              std::string prefix) const;
    void run_experiment_const(pcg32 &rng,
                              std::vector<Technique<Float> *> techniques,
                              std::string prefix) const;
    void run_experiment_scos(pcg32 &rng,
                             std::vector<Technique<Float> *> techniques,
                             std::string prefix) const;
    void run_experiment_gauss(pcg32 &rng,
                              std::vector<Technique<Float> *> techniques,
                              std::string prefix) const;
    virtual std::string getName() const { return "exp_Heat"; }
    virtual std::string getDataPath() const
    {
        return "../results/" + getName() + "/";
    }
    virtual void preProcess()
    {
        std::string cmd = "mkdir " + getDataPath();
        system(cmd.c_str());
    }
};

class ReciprocalHeatPlots : public Experiment
{
public:
    virtual void run() const;
    virtual void run(int test) const;
    void run_experiment_noise(pcg32 &rng,
                              std::vector<Technique<Float> *> techniques,
                              std::string prefix) const;
    void run_experiment_const(pcg32 &rng,
                              std::vector<Technique<Float> *> techniques,
                              std::string prefix) const;
    void run_experiment_scos(pcg32 &rng,
                             std::vector<Technique<Float> *> techniques,
                             std::string prefix) const;
    void run_experiment_gauss(pcg32 &rng,
                              std::vector<Technique<Float> *> techniques,
                              std::string prefix) const;
    virtual std::string getName() const { return "recip_Heat"; }
    virtual std::string getDataPath() const
    {
        return "../results/" + getName() + "/";
    }
    virtual void preProcess()
    {
        std::string cmd = "mkdir " + getDataPath();
        system(cmd.c_str());
    }
};

class PinkDavisHeatPlots : public Experiment
{
public:
    virtual void run() const;
    virtual void run(int test) const;
    void run_experiment_noise(pcg32 &rng,
                              Float c,
                              std::vector<Technique<Float> *> techniques,
                              std::string prefix) const;
    void run_experiment_const(pcg32 &rng,
                              Float c,
                              std::vector<Technique<Float> *> techniques,
                              std::string prefix) const;
    void run_experiment_scos(pcg32 &rng,
                             Float c,
                             std::vector<Technique<Float> *> techniques,
                             std::string prefix) const;
    void run_experiment_gauss(pcg32 &rng,
                              Float c,
                              std::vector<Technique<Float> *> techniques,
                              std::string prefix) const;
    virtual std::string getName() const { return "pink_Heat"; }
    virtual std::string getDataPath() const
    {
        return "../results/" + getName() + "/";
    }
    virtual void preProcess()
    {
        std::string cmd = "mkdir " + getDataPath();
        system(cmd.c_str());
    }
};

class PhotonMappingDensityEstDebVsPPM : public Experiment
{
public:
    virtual void run() const;
    virtual void run(int test_ind) const;
    void run_experiment(pcg32 &rng, std::string prefix) const;
    void run_experiment_ind(pcg32 &seed_rng, int index, std::string prefix) const;
    Float ppm(pcg32 &rng,
              Float base_radius,
              int base_photons,
              std::string pre_fix,
              std::string post_fix) const;
    Float deb_uni_square(pcg32 &rng,
                         Float base_radius,
                         Float max_cost,
                         int base_photons,
                         std::string pre_fix,
                         std::string post_fix) const;
    Float deb_cone_square(pcg32 &rng,
                          Float base_radius,
                          Float max_cost,
                          int base_photons,
                          std::string pre_fix,
                          std::string post_fix) const;
    Float deb_quad_square(pcg32 &rng,
                          Float base_radius,
                          Float max_cost,
                          int base_photons,
                          std::string pre_fix,
                          std::string post_fix) const;
    Float deb_quart_square(pcg32 &rng,
                           Float base_radius,
                           Float max_cost,
                           int base_photons,
                           std::string pre_fix,
                           std::string post_fix) const;
    Float deb_cone_alpha(pcg32 &rng,
                         Float base_radius,
                         Float max_cost,
                         Float alpha,
                         Float c,
                         int base_photons,
                         std::string pre_fix,
                         std::string post_fix) const;
    Float debprog_cone_alpha(pcg32 &rng,
                             Float base_radius,
                             Float max_cost,
                             Float alpha,
                             Float c,
                             int base_photons,
                             std::string pre_fix,
                             std::string post_fix) const;
    virtual std::string getName() const { return "photon_deb_vs_ppm"; }
    virtual std::string getDataPath() const
    {
        return "../results/" + getName() + "/";
    }
    virtual void preProcess()
    {
        std::string cmd = "mkdir " + getDataPath();
        system(cmd.c_str());
    }
};

// keep batch version
class PowerLawCIConvergenceExperiments : public Experiment
{
public:
    virtual void run() const;
    virtual void run(int test_index) const;
    void run_experiment(pcg32 &rng) const;
    void run_experiment_ind(pcg32 &rng, int test_index) const;
    void ci_test(pcg32 &rng, std::string loc, Float alpha, int trials, int samples) const;
    // Float pow_law(pcg32 &rng,
    //               Float alpha) const;

    virtual std::string getName() const { return "power_law_ci"; }
    virtual std::string getDataPath() const
    {
        return "../results/" + getName() + "/";
    }
    virtual void preProcess()
    {
        std::string cmd = "mkdir " + getDataPath();
        system(cmd.c_str());
    }
};