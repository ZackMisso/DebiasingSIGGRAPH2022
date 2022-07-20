#include <conver/experiment_manager.h>

void ExperimentManager::run(Experiment *experiment)
{
    experiment->preProcess();
    experiment->run();
    delete experiment;
}

void ExperimentManager::run(Experiment *experiment, int test_ind)
{
    experiment->preProcess();
    experiment->run(test_ind);
    delete experiment;
}

void ExperimentManager::runExperiment(int argc, char *argv[])
{
    if (std::string(argv[1]) == "--recip_heat")
    {
        run(new ReciprocalHeatPlots());
        return;
    }

    if (std::string(argv[1]) == "--pink_heat")
    {
        run(new PinkDavisHeatPlots());
        return;
    }

    if (std::string(argv[1]) == "--exp_heat")
    {
        run(new ExponentialHeatPlots());
        return;
    }

    if (std::string(argv[1]) == "--recip_heat_ind")
    {
        int test_ind = std::stoi(argv[2]);
        run(new ReciprocalHeatPlots(), test_ind);
        return;
    }

    if (std::string(argv[1]) == "--pink_heat_ind")
    {
        int test_ind = std::stoi(argv[2]);
        run(new PinkDavisHeatPlots(), test_ind);
        return;
    }

    if (std::string(argv[1]) == "--exp_heat_ind")
    {
        int test_ind = std::stoi(argv[2]);
        run(new ExponentialHeatPlots(), test_ind);
        return;
    }

    if (std::string(argv[1]) == "--photon_equal_cost")
    {
        PhotonMappingDensityEstDebVsPPM *experiment = new PhotonMappingDensityEstDebVsPPM();
        experiment->preProcess();
        experiment->run();
        delete experiment;
        return;
    }

    if (std::string(argv[1]) == "--photon_equal_cost_ind")
    {
        int test_ind = std::stoi(argv[2]);
        PhotonMappingDensityEstDebVsPPM *experiment = new PhotonMappingDensityEstDebVsPPM();
        experiment->preProcess();
        experiment->run(test_ind);
        delete experiment;
        return;
    }

    if (std::string(argv[1]) == "--photon_equal_cost_comp")
    {
        int max_iter = 1024;
        int total_threads = 8;
        int start_index = std::stoi(argv[2]);
        int start = max_iter / total_threads * start_index;
        int end = max_iter / total_threads * (start_index + 1);

        for (int i = start; i < end; ++i)
        {
            PhotonMappingDensityEstDebVsPPM *experiment = new PhotonMappingDensityEstDebVsPPM();
            experiment->preProcess();
            experiment->run(i);
            delete experiment;
        }
        return;
    }

    if (std::string(argv[1]) == "--pow_law_ci")
    {
        PowerLawCIConvergenceExperiments *experiment = new PowerLawCIConvergenceExperiments();
        experiment->preProcess();
        experiment->run();
        delete experiment;
        return;
    }

    if (std::string(argv[1]) == "--pow_law_ci_ind")
    {
        int test_ind = std::stoi(argv[2]);
        PowerLawCIConvergenceExperiments *experiment = new PowerLawCIConvergenceExperiments();
        experiment->preProcess();
        experiment->run(test_ind);
        delete experiment;
        return;
    }

    std::cout << "unrecognized experiment name: " << std::string(argv[1]) << std::endl;
}
