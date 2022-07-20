#include <conver/experiment.h>
#include <conver/simple_funcs.h>
#include <conver/funcs/bin_func.h>
#include <conver/grapher.h>
#include <conver/problem.h>
#include <conver/technique.h>
#include <conver/sample_interval.h>
#include <conver/conv_eval.h>
#include <conver/techniques/all_techs.h>

Float PhotonMappingDensityEstDebVsPPM::ppm(pcg32 &rng,
                                           Float base_radius,
                                           int base_photons,
                                           std::string pre_fix,
                                           std::string post_fix) const
{

    Float alpha = 2.0 / 3.0;
    Float rad = base_radius;
    Float res = 0.0;
    Float var = 0.0;

    PhotonProblem *prob = new PhotonProblem();

    int total_steps = PPMSteps;

    Float expected = prob->expected();

    std::ofstream file;
    file.open(getDataPath() + pre_fix + "equal_cost_ppm" + post_fix + ".txt");

    Float total_cost = 0.0;

    for (int i = 0; i < total_steps; ++i)
    {
        rad = base_radius * std::sqrt(std::pow(Float(i + 1), alpha - 1.0));

        Float base = 0.0;
        for (int k = 0; k < base_photons; ++k)
        {
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

            Float dist_to_ground = -prob->get_variable(PHOTON_EMITTER_Z) / zdir;

            Float xloc = xdir * dist_to_ground;
            Float yloc = ydir * dist_to_ground;

            Float dist = std::sqrt(xloc * xloc + yloc * yloc);

            Float energy = prob->get_variable(PHOTON_EMITTER_POWER) / Float(base_photons);

            if (dist <= rad)
                base += energy * (1.0 - dist / rad);
        }

        base /= M_PI * rad * rad * 1.0 / 3.0;

        Float base_eval = std::abs(base - expected);
        res = res + (1.0 / Float(i + 1)) * (base_eval - res);
        Float var_eval = (base - expected) * (base - expected);
        var = var + (1.0 / Float(i + 1)) * (var_eval - var);

        total_cost += base_photons;

        if (i % 5 == 0)
        {
            file << i << std::endl;
            file << total_cost << std::endl;
            file << res << std::endl;
            file << var << std::endl;
        }

        if (i % 1000 == 0)
            std::cout << "iter: " << i << " res: " << res << " rad: " << rad << std::endl;
    }

    file.close();

    delete prob;

    return res;
}

Float PhotonMappingDensityEstDebVsPPM::deb_uni_square(pcg32 &rng,
                                                      Float base_radius,
                                                      Float max_cost,
                                                      int base_photons,
                                                      std::string pre_fix,
                                                      std::string post_fix) const
{
    PhotonProblem *problem = new PhotonProblem();
    Float expected = problem->expected();

    PhotonSquareUni *tele = new PhotonSquareUni(0.65, base_radius, base_photons, 1);

    std::ofstream file;
    file.open(getDataPath() + pre_fix + "equal_cost_deb_uni_square" + post_fix + ".txt");

    Float res = 0.0;
    Float var = 0.0;
    Float current_cost = 0.0;
    int i = 0;

    while (max_cost > current_cost)
    {
        int tmp_cost = 0;
        Float eval = tele->eval_problem(problem, 1, tmp_cost, rng);
        Float eval_base = std::abs(eval - expected);
        res = res + 1.0 / Float(i + 1) * (eval_base - res);
        Float var_eval = (eval - expected) * (eval - expected);
        var = var + 1.0 / Float(i + 1) * (var_eval - var);
        current_cost += tmp_cost;

        file << i << std::endl;
        file << current_cost << std::endl;
        file << res << std::endl;
        file << var << std::endl;

        if (i % 1000 == 0)
            std::cout << "iter: " << i << " current_cost: " << current_cost << std::endl;

        ++i;
    }

    file.close();

    delete tele;
    delete problem;

    return res;
}

Float PhotonMappingDensityEstDebVsPPM::deb_cone_square(pcg32 &rng,
                                                       Float base_radius,
                                                       Float max_cost,
                                                       int base_photons,
                                                       std::string pre_fix,
                                                       std::string post_fix) const
{
    PhotonProblem *problem = new PhotonProblem();
    Float expected = problem->expected();

    PhotonSquareCone *tele = new PhotonSquareCone(0.65, base_radius, base_photons, 1);

    std::ofstream file;
    file.open(getDataPath() + pre_fix + "equal_cost_deb_cone_square" + post_fix + ".txt");

    Float res = 0.0;
    Float var = 0.0;
    Float current_cost = 0.0;
    int i = 0;

    while (max_cost > current_cost)
    {
        int tmp_cost = 0;
        Float eval = tele->eval_problem(problem, 1, tmp_cost, rng);
        Float eval_base = std::abs(eval - expected);
        res = res + 1.0 / Float(i + 1) * (eval_base - res);
        Float var_eval = (eval - expected) * (eval - expected);
        var = var + 1.0 / Float(i + 1) * (var_eval - var);
        current_cost += tmp_cost;

        file << i << std::endl;
        file << current_cost << std::endl;
        file << res << std::endl;
        file << var << std::endl;

        if (i % 1000 == 0)
            std::cout << "iter: " << i << " current_cost: " << current_cost << std::endl;

        ++i;
    }

    file.close();

    delete tele;
    delete problem;

    return res;
}

Float PhotonMappingDensityEstDebVsPPM::deb_cone_alpha(pcg32 &rng,
                                                      Float base_radius,
                                                      Float max_cost,
                                                      Float alpha,
                                                      Float c,
                                                      int base_photons,
                                                      std::string pre_fix,
                                                      std::string post_fix) const
{
    PhotonProblem *problem = new PhotonProblem();
    Float expected = problem->expected();

    PhotonAlphaCone *tele = new PhotonAlphaCone(alpha, c, base_radius, base_photons, 1);

    std::ofstream file;
    file.open(getDataPath() + pre_fix + "equal_cost_deb_cone_alpha" + post_fix + ".txt");

    Float res = 0.0;
    Float var = 0.0;
    Float current_cost = 0.0;
    int i = 0;

    while (max_cost > current_cost)
    {
        int tmp_cost = 0;
        Float eval = tele->eval_problem(problem, 1, tmp_cost, rng);
        Float eval_base = std::abs(eval - expected);
        res = res + 1.0 / Float(i + 1) * (eval_base - res);
        Float var_eval = (eval - expected) * (eval - expected);
        var = var + 1.0 / Float(i + 1) * (var_eval - var);
        current_cost += tmp_cost;

        file << i << std::endl;
        file << current_cost << std::endl;
        file << res << std::endl;
        file << var << std::endl;

        if (i % 1000 == 0)
            std::cout << "iter: " << i << " current_cost: " << current_cost << std::endl;

        ++i;
    }

    file.close();

    delete tele;
    delete problem;

    return res;
}

Float PhotonMappingDensityEstDebVsPPM::debprog_cone_alpha(pcg32 &rng,
                                                          Float base_radius,
                                                          Float max_cost,
                                                          Float alpha,
                                                          Float c,
                                                          int base_photons,
                                                          std::string pre_fix,
                                                          std::string post_fix) const
{
    PhotonProblem *problem = new PhotonProblem();
    Float expected = problem->expected();

    PhotonProgAlphaCone *tele = new PhotonProgAlphaCone(alpha, c, base_radius, base_photons, 1);

    std::ofstream file;
    file.open(getDataPath() + pre_fix + "equal_cost_debprog_cone_alpha" + post_fix + ".txt");

    Float res = 0.0;
    Float var = 0.0;
    Float current_cost = 0.0;
    int i = 0;

    while (max_cost > current_cost)
    {
        int tmp_cost = 0;
        Float eval = tele->eval_problem(problem, i + 1, tmp_cost, rng);
        Float eval_base = std::abs(eval - res);
        res = res + 1.0 / Float(i + 1) * (eval_base - res);
        Float var_eval = (eval - expected) * (eval - expected);
        var = var + 1.0 / Float(i + 1) * (var_eval - var);
        current_cost += tmp_cost;

        file << i << std::endl;
        file << current_cost << std::endl;
        file << res << std::endl;
        file << var << std::endl;

        if (i % 1000 == 0)
            std::cout << "iter: " << i << " current_cost: " << current_cost << std::endl;

        ++i;
    }

    file.close();

    delete tele;
    delete problem;

    return res;
}

Float PhotonMappingDensityEstDebVsPPM::deb_quad_square(pcg32 &rng,
                                                       Float base_radius,
                                                       Float max_cost,
                                                       int base_photons,
                                                       std::string pre_fix,
                                                       std::string post_fix) const
{
    PhotonProblem *problem = new PhotonProblem();
    Float expected = problem->expected();

    PhotonSquareQuad *tele = new PhotonSquareQuad(0.65, base_radius, base_photons, 1);

    std::ofstream file;
    file.open(getDataPath() + pre_fix + "equal_cost_deb_quad_square" + post_fix + ".txt");

    Float res = 0.0;
    Float var = 0.0;
    Float current_cost = 0.0;
    int i = 0;

    while (max_cost > current_cost)
    {
        int tmp_cost = 0;
        Float eval = tele->eval_problem(problem, 1, tmp_cost, rng);
        // eval = std::abs(eval - expected);

        res = res + 1.0 / Float(i + 1) * (eval - res);
        eval *= eval;
        var = var + 1.0 / Float(i + 1) * (eval - var);
        current_cost += tmp_cost;

        file << i << std::endl;
        file << current_cost << std::endl;
        file << std::abs(res - expected) << std::endl;
        file << var << std::endl;

        if (i % 1000 == 0)
            std::cout << "iter: " << i << " current_cost: " << current_cost << std::endl;

        ++i;
    }

    file.close();

    delete tele;
    delete problem;

    return res;
}

Float PhotonMappingDensityEstDebVsPPM::deb_quart_square(pcg32 &rng,
                                                        Float base_radius,
                                                        Float max_cost,
                                                        int base_photons,
                                                        std::string pre_fix,
                                                        std::string post_fix) const
{
    PhotonProblem *problem = new PhotonProblem();
    Float expected = problem->expected();

    PhotonSquareQuart *tele = new PhotonSquareQuart(0.65, base_radius, base_photons, 1);

    std::ofstream file;
    file.open(getDataPath() + pre_fix + "equal_cost_deb_quart_square" + post_fix + ".txt");

    Float res = 0.0;
    Float var = 0.0;
    Float current_cost = 0.0;
    int i = 0;

    while (max_cost > current_cost)
    {
        int tmp_cost = 0;
        Float eval = tele->eval_problem(problem, 1, tmp_cost, rng);
        eval = std::abs(eval - expected);

        res = res + 1.0 / Float(i + 1) * (eval - res);
        eval *= eval;
        var = var + 1.0 / Float(i + 1) * (eval - var);
        current_cost += tmp_cost;

        file << i << std::endl;
        file << current_cost << std::endl;
        file << res << std::endl;
        file << var << std::endl;

        if (i % 1000 == 0)
            std::cout << "iter: " << i << " current_cost: " << current_cost << std::endl;

        ++i;
    }

    file.close();

    delete tele;
    delete problem;

    return res;
}

void PhotonMappingDensityEstDebVsPPM::run_experiment(pcg32 &rng,
                                                     std::string prefix) const
{
    // create the necessary storage folder
    std::string path = getDataPath() + prefix;
    std::string cmd = "mkdir " + path;
    system(cmd.c_str());

    Float radius = PPMRadius;
    int base_cost = PPMBasePhotons;

    // full experiment
    // ppm(rng, radius, base_cost, prefix, "_0");
    Float alpha = 2.0 / 3.0;
    Float c = (2.0 * alpha - 1.0) / alpha - 0.001;
    deb_cone_alpha(rng, radius, Float(PPMSteps) * Float(base_cost), alpha, c, base_cost, prefix, "_inf_work_0");
}

void PhotonMappingDensityEstDebVsPPM::run_experiment_ind(pcg32 &seed_rng,
                                                         int test_index,
                                                         std::string prefix) const
{
    uint32_t seedOne = 0;
    uint32_t seedTwo = 0;

    for (int i = 0; i <= test_index; ++i)
    {
        seedOne = seed_rng.nextUInt();
        seedTwo = seed_rng.nextUInt();
    }

    pcg32 rng = pcg32(seedOne, seedTwo);

    // create the necessary storage folder
    std::string path = getDataPath() + prefix;
    std::string cmd = "mkdir " + path;
    system(cmd.c_str());

    Float radius = PPMRadius;
    int base_cost = PPMBasePhotons;

    int index = test_index / 3;
    int test = test_index % 3;

    // compare different alpha convergences
    if (test == 0)
        ppm(rng, radius, base_cost, prefix, "_" + std::to_string(index));
    if (test == 1)
    {
        Float alpha = 2.0 / 3.0;
        Float c = 1.0001;
        deb_cone_alpha(rng, radius, Float(PPMSteps) * Float(base_cost), alpha, c, base_cost, prefix, "_final_" + std::to_string(index));
    }
    if (test == 2)
    {
        Float alpha = 2.0 / 3.0;
        Float c = 1.0001;
        debprog_cone_alpha(rng, radius, Float(PPMSteps) * Float(base_cost), alpha, c, base_cost, prefix, "_final_" + std::to_string(index));
    }
}

void PhotonMappingDensityEstDebVsPPM::run() const
{
    pcg32 rng = pcg32(0x837, 0x10c9);

    std::cout << "running test: " << getName() << std::endl;
    std::string test = "full_test/";

    run_experiment(rng, test);
}

void PhotonMappingDensityEstDebVsPPM::run(int test_ind) const
{
    pcg32 rng = pcg32(0x9232, 0xacd3);

    std::cout << "running test: " << getName() << std::endl;
    std::string test = "full_test/";

    run_experiment_ind(rng, test_ind, test);
}