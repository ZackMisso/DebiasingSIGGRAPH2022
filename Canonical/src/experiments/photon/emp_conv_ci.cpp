#include <conver/experiment.h>
#include <conver/simple_funcs.h>
#include <conver/funcs/bin_func.h>
#include <conver/grapher.h>
#include <conver/problem.h>
#include <conver/technique.h>
#include <conver/sample_interval.h>
#include <conver/conv_eval.h>
#include <conver/techniques/all_techs.h>

void PowerLawCIConvergenceExperiments::ci_test(pcg32 &rng, std::string loc, Float alpha, int trials, int samples) const
{
    std::vector<Float> evals = std::vector<Float>();

    PowerLawFunction pow_func = PowerLawFunction(alpha, 0.0, 1.0);
    IntegralEval problem = IntegralEval(&pow_func);
    TestMC<Float> evaluator = TestMC<Float>();

    Float expected = problem.expected();

    for (int i = 0; i < trials; ++i)
    {
        // if (i % 1000 == 0)
        //     std::cout << "Running trial: " << i << std::endl;
        Float mean = 0.0;

        for (int j = 0; j < samples; ++j)
        {
            int tmp_cost;
            Float eval = evaluator.eval_problem(&problem, 1, tmp_cost, rng) - expected;
            mean = mean + 1.0 / Float(1 + j) * (eval - mean);
        }

        evals.push_back(mean);
    }

    Float max_value = -1e100;
    Float min_value = 1e100;

    for (int i = 0; i < evals.size(); ++i)
    {
        if (evals[i] < min_value)
            min_value = evals[i];
        if (evals[i] > max_value)
            max_value = evals[i];
    }

    // std::vector<Float> bins = std::vector<Float>(boxes);

    // for (int i = 0; i < bins.size(); ++i)
    //     bins[i] = 0.0;

    // for (int i = 0; i < evals.size(); ++i)
    // {
    //     int loc = std::floor((evals[i] - min_value) / (max_value - min_value) * Float(boxes));
    //     bins[loc] += 1.0;
    // }

    // for (int i = 0; i < bins.size(); ++i)
    //     bins[i] /= Float(evals.size());

    std::ofstream data;
    data.open(getDataPath() + loc + std::to_string(samples) + "_samples.txt");

    for (int i = 0; i < evals.size(); ++i)
    {
        data << evals[i] << std::endl;
    }

    data.close();
}

void PowerLawCIConvergenceExperiments::run_experiment(pcg32 &rng) const
{
    Float alpha = 2.0 / 3.0;
    int trials = 1024 * 256;

    std::cout << "running ci_test for samples: 1" << std::endl;
    ci_test(rng, "", alpha, trials, 1);
    std::cout << "running ci_test for samples: 2" << std::endl;
    ci_test(rng, "", alpha, trials, 2);
    std::cout << "running ci_test for samples: 4" << std::endl;
    ci_test(rng, "", alpha, trials, 4);
    std::cout << "running ci_test for samples: 8" << std::endl;
    ci_test(rng, "", alpha, trials, 8);
    std::cout << "running ci_test for samples: 16" << std::endl;
    ci_test(rng, "", alpha, trials, 16);
    std::cout << "running ci_test for samples: 32" << std::endl;
    ci_test(rng, "", alpha, trials, 32);
    std::cout << "running ci_test for samples: 64" << std::endl;
    ci_test(rng, "", alpha, trials, 64);
    std::cout << "running ci_test for samples: 128" << std::endl;
    ci_test(rng, "", alpha, trials, 128);
    std::cout << "running ci_test for samples: 256" << std::endl;
    ci_test(rng, "", alpha, trials, 256);
    std::cout << "running ci_test for samples: 512" << std::endl;
    ci_test(rng, "", alpha, trials, 512);
    std::cout << "running ci_test for samples: 1024" << std::endl;
    ci_test(rng, "", alpha, trials, 1024);
    std::cout << "running ci_test for samples: 2048" << std::endl;
    ci_test(rng, "", alpha, trials, 2048);
    std::cout << "running ci_test for samples: 4096" << std::endl;
    ci_test(rng, "", alpha, trials, 4096);
    std::cout << "running ci_test for samples: 8192" << std::endl;
    ci_test(rng, "", alpha, trials, 8192);
    std::cout << "running ci_test for samples: 16384" << std::endl;
    ci_test(rng, "", alpha, trials, 16384);
    std::cout << "running ci_test for samples: 32768" << std::endl;
    ci_test(rng, "", alpha, trials, 32768);
    std::cout << "running ci_test for samples: 65536" << std::endl;
    ci_test(rng, "", alpha, trials, 65536);
    std::cout << "running ci_test for samples: 131072" << std::endl;
    ci_test(rng, "", alpha, trials, 131072);
    std::cout << "running ci_test for samples: 262144" << std::endl;
    ci_test(rng, "", alpha, trials, 262144);
}

void PowerLawCIConvergenceExperiments::run_experiment_ind(pcg32 &rng, int test_index) const
{
    // Float alpha = 2.0 / 3.0;
    int trials = 1024 * 128;

    Float min_alpha = 0.2;
    Float max_alpha = 0.91;
    Float alpha_change = 0.01;

    // int total_tests = (max_alpha - min_alpha) / alpha_change;
    int total_samps = 18 + 6;

    int alpha_ind = test_index / total_samps;
    int samp_ind = test_index % total_samps;

    Float alpha = Float(alpha_ind) * alpha_change + min_alpha;
    int samples = std::pow(2, samp_ind);

    std::string mkdir_path = "mkdir " + getDataPath() + "alpha_" + std::to_string(alpha_ind) + "/";
    system(mkdir_path.c_str());

    ci_test(rng, "alpha_" + std::to_string(alpha_ind) + "/", alpha, trials, samples);
}

void PowerLawCIConvergenceExperiments::run() const
{
    pcg32 rng = pcg32(0x2395, 0xc109);

    run_experiment(rng);
}

void PowerLawCIConvergenceExperiments::run(int index) const
{
    pcg32 seed_rng = pcg32(0x2395, 0xc109);

    for (int i = 0; i < index; ++i)
    {
        seed_rng.nextUInt();
        seed_rng.nextUInt();
    }

    pcg32 rng = pcg32(seed_rng.nextUInt(), seed_rng.nextUInt());

    // 1278 total tests
    run_experiment_ind(rng, index);
}
