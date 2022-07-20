#include <conver/experiment.h>
#include <conver/simple_funcs.h>
#include <conver/funcs/bin_func.h>
#include <conver/grapher.h>
#include <conver/problem.h>
#include <conver/technique.h>
#include <conver/sample_interval.h>
#include <conver/conv_eval.h>
#include <conver/techniques/all_techs.h>

#include <imedit/image.h>

void PinkDavisHeatPlots::run_experiment_noise(
    pcg32 &rng,
    Float c,
    std::vector<Technique<Float> *> techniques,
    std::string prefix) const
{
    // create the necessary storage folder
    std::string path = getDataPath() + prefix;
    std::string cmd = "mkdir " + path;
    system(cmd.c_str());

    // graph the results
    GraphSettings loglog = GraphSettings();
    loglog.x_log = false;
    loglog.y_log = false;

    std::vector<ConvergenceData> conv_data = std::vector<ConvergenceData>();
    std::vector<double> vals = std::vector<double>();

    int trials = HeatTrials;
    int res = HeatRes;

    double min_p = 0.01;
    double max_p = 0.99;
    double min_yx = 1.0;
    double max_yx = 2.0;

    std::vector<imedit::Image *> images = std::vector<imedit::Image *>();
    for (int i = 0; i < techniques.size(); ++i)
    {
        images.push_back(new imedit::Image(res, res)); // mean
        images.push_back(new imedit::Image(res, res)); // err
        images.push_back(new imedit::Image(res, res)); // mse
        images.push_back(new imedit::Image(res, res)); // work
        images.push_back(new imedit::Image(res, res)); // work_norm_var
    }

    imedit::Image mean_image = imedit::Image(res, res);

    // TODO: create a randomized sine function
    CompositeSineFunction comp_func = CompositeSineFunction(rng, 10, 0.0, 1.0);
    comp_func.normalize();

    Grapher::graphFunction(comp_func, path + "comp_func.png");
    std::cout << "comp func integral: " << comp_func.integrate() << std::endl;

    int i = 0;
    int j = 0;

    for (double p = min_p; (p < max_p) && (j < res); p += (max_p - min_p) / Float(res))
    {
        // this will only work for sin_functions
        i = 0;
        std::cout << "Evaluating col: " << j << std::endl;

        // initialize sine function
        CompositeSineFunction comp_iter_func = comp_func;

        Float mean = (std::pow(p, -c * c) - 1) / (c * c);

        comp_iter_func.solve_for_mean(mean);
        Float max = comp_iter_func.compute_max();

        // transmittance should be x
        for (double yx = min_yx; (yx < max_yx) && (i < res); yx += (max_yx - min_yx) / Float(res))
        {
            Float maj = max * yx;

            for (int k = 0; k < techniques.size(); ++k)
                techniques[k]->setMaj(maj);

            PinkDavis recip = PinkDavis(&comp_iter_func, c);

            // initialize a sample interval to evaluate the convergence of
            PowerTwoSampleInterval sample_interval_short = PowerTwoSampleInterval(trials, HeatSamples);

            ConvergenceEvaluatorHeat<Float> evaluator = ConvergenceEvaluatorHeat<Float>(&recip, techniques, &sample_interval_short, rng.nextUInt(), rng.nextUInt());
            evaluator.evaluate();

            // create convergence data
            ConvergenceData avg_data = ConvergenceData(evaluator.getData()->data_records);

            for (int k = 0; k < techniques.size(); ++k)
            {
                int data_pts = avg_data.getRecordByIndex(k).data_points.size();

                images[5 * k + 0]->setPixel(j, i, imedit::Pixel(avg_data.getRecordByIndex(k).data_points[data_pts - 1].mean));
                images[5 * k + 1]->setPixel(j, i, imedit::Pixel(avg_data.getRecordByIndex(k).data_points[data_pts - 1].bias));
                images[5 * k + 2]->setPixel(j, i, imedit::Pixel(avg_data.getRecordByIndex(k).data_points[data_pts - 1].true_mse));
                images[5 * k + 3]->setPixel(j, i, imedit::Pixel(avg_data.getRecordByIndex(k).data_points[data_pts - 1].work));
                images[5 * k + 4]->setPixel(j, i, imedit::Pixel(avg_data.getRecordByIndex(k).data_points[data_pts - 1].work_norm_var));
            }

            mean_image.setPixel(j, i, imedit::Pixel(recip.eval(comp_iter_func.integrate())));

            i++;
        }
        j++;
    }

    for (int k = 0; k < techniques.size(); ++k)
    {
        std::cout << "path: " << getDataPath() + prefix + techniques[k]->getName() + "_mean.exr" << std::endl;
        images[5 * k + 0]->write(getDataPath() + prefix + techniques[k]->getName() + "_mean.exr");
        images[5 * k + 1]->write(getDataPath() + prefix + techniques[k]->getName() + "_err.exr");
        images[5 * k + 2]->write(getDataPath() + prefix + techniques[k]->getName() + "_mse.exr");
        images[5 * k + 3]->write(getDataPath() + prefix + techniques[k]->getName() + "_work.exr");
        images[5 * k + 4]->write(getDataPath() + prefix + techniques[k]->getName() + "_work_norm_var.exr");
    }

    mean_image.write(getDataPath() + prefix + "gt_mean.exr");
}

void PinkDavisHeatPlots::run_experiment_const(
    pcg32 &rng,
    Float c,
    std::vector<Technique<Float> *> techniques,
    std::string prefix) const
{
    // create the necessary storage folder
    std::string path = getDataPath() + prefix;
    std::string cmd = "mkdir " + path;
    system(cmd.c_str());

    // graph the results
    GraphSettings loglog = GraphSettings();
    loglog.x_log = false;
    loglog.y_log = false;

    std::vector<ConvergenceData> conv_data = std::vector<ConvergenceData>();
    std::vector<double> vals = std::vector<double>();

    int trials = HeatTrials;
    int res = HeatRes;

    double min_p = 0.01;
    double max_p = 0.99;
    double min_yx = 1.0;
    double max_yx = 2.0;

    std::vector<imedit::Image *> images = std::vector<imedit::Image *>();
    for (int i = 0; i < techniques.size(); ++i)
    {
        images.push_back(new imedit::Image(res, res)); // mean
        images.push_back(new imedit::Image(res, res)); // err
        images.push_back(new imedit::Image(res, res)); // mse
        images.push_back(new imedit::Image(res, res)); // work
        images.push_back(new imedit::Image(res, res)); // work_norm_var
    }

    imedit::Image mean_image = imedit::Image(res, res);

    // TODO: create a randomized sine function
    ConstantFunction comp_func = ConstantFunction(1.0, 0.0, 1.0);

    Grapher::graphFunction(comp_func, path + "comp_func.png");
    std::cout << "comp func integral: " << comp_func.integrate() << std::endl;

    int i = 0;
    int j = 0;

    for (double p = min_p; (p < max_p) && (j < res); p += (max_p - min_p) / Float(res))
    {
        // this will only work for sin_functions
        i = 0;
        std::cout << "Evaluating col: " << j << std::endl;

        // initialize sine function
        ConstantFunction comp_iter_func = comp_func;

        Float mean = (std::pow(p, -c * c) - 1) / (c * c);

        comp_iter_func.solve_for_mean(mean);
        Float max = comp_iter_func.compute_max(); // compute max was broken, ugh

        for (double yx = min_yx; (yx < max_yx) && (i < res); yx += (max_yx - min_yx) / Float(res))
        {
            Float maj = max * yx;

            for (int k = 0; k < techniques.size(); ++k)
                techniques[k]->setMaj(maj);

            PinkDavis recip = PinkDavis(&comp_iter_func, c);

            // initialize a sample interval to evaluate the convergence of
            PowerTwoSampleInterval sample_interval_short = PowerTwoSampleInterval(trials, HeatSamples);

            ConvergenceEvaluatorHeat<Float> evaluator = ConvergenceEvaluatorHeat<Float>(&recip, techniques, &sample_interval_short, rng.nextUInt(), rng.nextUInt());
            evaluator.evaluate();

            std::vector<ConvergenceRecord> data_records = std::vector<ConvergenceRecord>();

            // create convergence data
            ConvergenceData avg_data = ConvergenceData(evaluator.getData()->data_records);

            for (int k = 0; k < techniques.size(); ++k)
            {
                int data_pts = avg_data.getRecordByIndex(k).data_points.size();

                images[5 * k + 0]->setPixel(j, i, imedit::Pixel(avg_data.getRecordByIndex(k).data_points[data_pts - 1].mean));
                images[5 * k + 1]->setPixel(j, i, imedit::Pixel(avg_data.getRecordByIndex(k).data_points[data_pts - 1].bias));
                images[5 * k + 2]->setPixel(j, i, imedit::Pixel(avg_data.getRecordByIndex(k).data_points[data_pts - 1].true_mse));
                images[5 * k + 3]->setPixel(j, i, imedit::Pixel(avg_data.getRecordByIndex(k).data_points[data_pts - 1].work));
                images[5 * k + 4]->setPixel(j, i, imedit::Pixel(avg_data.getRecordByIndex(k).data_points[data_pts - 1].work_norm_var));
            }

            mean_image.setPixel(j, i, imedit::Pixel(recip.eval(comp_iter_func.integrate())));

            i++;
        }
        j++;
    }

    for (int k = 0; k < techniques.size(); ++k)
    {
        std::cout << "path: " << getDataPath() + prefix + techniques[k]->getName() + "_mean.exr" << std::endl;
        images[5 * k + 0]->write(getDataPath() + prefix + techniques[k]->getName() + "_mean.exr");
        images[5 * k + 1]->write(getDataPath() + prefix + techniques[k]->getName() + "_err.exr");
        images[5 * k + 2]->write(getDataPath() + prefix + techniques[k]->getName() + "_mse.exr");
        images[5 * k + 3]->write(getDataPath() + prefix + techniques[k]->getName() + "_work.exr");
        images[5 * k + 4]->write(getDataPath() + prefix + techniques[k]->getName() + "_work_norm_var.exr");
    }

    mean_image.write(getDataPath() + prefix + "gt_mean.exr");
}

void PinkDavisHeatPlots::run_experiment_scos(
    pcg32 &rng,
    Float c,
    std::vector<Technique<Float> *> techniques,
    std::string prefix) const
{
    // create the necessary storage folder
    std::string path = getDataPath() + prefix;
    std::string cmd = "mkdir " + path;
    system(cmd.c_str());

    // graph the results
    GraphSettings loglog = GraphSettings();
    loglog.x_log = false;
    loglog.y_log = false;

    std::vector<ConvergenceData> conv_data = std::vector<ConvergenceData>();
    std::vector<double> vals = std::vector<double>();

    int trials = HeatTrials;
    int res = HeatRes;

    double min_p = 0.01;
    double max_p = 0.99;
    double min_yx = 1.0;
    double max_yx = 2.0;

    std::vector<imedit::Image *> images = std::vector<imedit::Image *>();
    for (int i = 0; i < techniques.size(); ++i)
    {
        images.push_back(new imedit::Image(res, res)); // mean
        images.push_back(new imedit::Image(res, res)); // err
        images.push_back(new imedit::Image(res, res)); // mse
        images.push_back(new imedit::Image(res, res)); // work
        images.push_back(new imedit::Image(res, res)); // work_norm_var
    }

    imedit::Image mean_image = imedit::Image(res, res);

    SineWaveFunction comp_func = SineWaveFunction(1.0, 1.0, 0.0, 0.0, 1.0);

    Grapher::graphFunction(comp_func, path + "comp_func.png");
    std::cout << "comp func integral: " << comp_func.integrate() << std::endl;

    int i = 0;
    int j = 0;

    for (double p = min_p; (p < max_p) && (j < res); p += (max_p - min_p) / Float(res))
    {
        // this will only work for sin_functions
        i = 0;
        std::cout << "Evaluating col: " << j << std::endl;

        SineWaveFunction comp_iter_func = comp_func;

        Float mean = (std::pow(p, -c * c) - 1) / (c * c);

        comp_iter_func.solve_for_mean(mean);
        Float max = comp_iter_func.compute_max();

        for (double yx = min_yx; (yx < max_yx) && (i < res); yx += (max_yx - min_yx) / Float(res))
        {
            // initialize sine function
            Float maj = max * yx;

            for (int k = 0; k < techniques.size(); ++k)
                techniques[k]->setMaj(maj);

            PinkDavis recip = PinkDavis(&comp_iter_func, c);

            // initialize a sample interval to evaluate the convergence of
            PowerTwoSampleInterval sample_interval_short = PowerTwoSampleInterval(trials, HeatSamples);

            ConvergenceEvaluatorHeat<Float> evaluator = ConvergenceEvaluatorHeat<Float>(&recip, techniques, &sample_interval_short, rng.nextUInt(), rng.nextUInt());
            evaluator.evaluate();

            // create convergence data
            ConvergenceData avg_data = ConvergenceData(evaluator.getData()->data_records);

            for (int k = 0; k < techniques.size(); ++k)
            {
                int data_pts = avg_data.getRecordByIndex(k).data_points.size();

                images[5 * k + 0]->setPixel(j, i, imedit::Pixel(avg_data.getRecordByIndex(k).data_points[data_pts - 1].mean));
                images[5 * k + 1]->setPixel(j, i, imedit::Pixel(avg_data.getRecordByIndex(k).data_points[data_pts - 1].bias));
                images[5 * k + 2]->setPixel(j, i, imedit::Pixel(avg_data.getRecordByIndex(k).data_points[data_pts - 1].true_mse));
                images[5 * k + 3]->setPixel(j, i, imedit::Pixel(avg_data.getRecordByIndex(k).data_points[data_pts - 1].work));
                images[5 * k + 4]->setPixel(j, i, imedit::Pixel(avg_data.getRecordByIndex(k).data_points[data_pts - 1].work_norm_var));
            }

            mean_image.setPixel(j, i, imedit::Pixel(recip.eval(comp_iter_func.integrate())));

            i++;
        }
        j++;
    }

    for (int k = 0; k < techniques.size(); ++k)
    {
        std::cout << "path: " << getDataPath() + prefix + techniques[k]->getName() + "_mean.exr" << std::endl;
        images[5 * k + 0]->write(getDataPath() + prefix + techniques[k]->getName() + "_mean.exr");
        images[5 * k + 1]->write(getDataPath() + prefix + techniques[k]->getName() + "_err.exr");
        images[5 * k + 2]->write(getDataPath() + prefix + techniques[k]->getName() + "_mse.exr");
        images[5 * k + 3]->write(getDataPath() + prefix + techniques[k]->getName() + "_work.exr");
        images[5 * k + 4]->write(getDataPath() + prefix + techniques[k]->getName() + "_work_norm_var.exr");
    }

    mean_image.write(getDataPath() + prefix + "gt_mean.exr");
}

void PinkDavisHeatPlots::run_experiment_gauss(
    pcg32 &rng,
    Float c,
    std::vector<Technique<Float> *> techniques,
    std::string prefix) const
{
    // create the necessary storage folder
    std::string path = getDataPath() + prefix;
    std::string cmd = "mkdir " + path;
    system(cmd.c_str());

    // graph the results
    GraphSettings loglog = GraphSettings();
    loglog.x_log = false;
    loglog.y_log = false;

    std::vector<ConvergenceData> conv_data = std::vector<ConvergenceData>();
    std::vector<double> vals = std::vector<double>();

    int trials = HeatTrials;
    int res = HeatRes;

    // std::cout << "RES: " << res << std::endl;

    double min_p = 0.01;
    double max_p = 0.99;
    double min_yx = 1.0;
    double max_yx = 2.0;

    std::vector<imedit::Image *> images = std::vector<imedit::Image *>();
    for (int i = 0; i < techniques.size(); ++i)
    {
        images.push_back(new imedit::Image(res, res)); // mean
        images.push_back(new imedit::Image(res, res)); // err
        images.push_back(new imedit::Image(res, res)); // mse
        images.push_back(new imedit::Image(res, res)); // work
        images.push_back(new imedit::Image(res, res)); // work_norm_var
    }

    imedit::Image mean_image = imedit::Image(res, res);

    GaussianFunction comp_func = GaussianFunction(1.0, 0.1, 0.4, 0.0, 1.0);

    Grapher::graphFunction(comp_func, path + "comp_func.png");
    std::cout << "comp func integral: " << comp_func.integrate() << std::endl;

    int i = 0;
    int j = 0;

    for (double p = min_p; (p < max_p) && (j < res); p += (max_p - min_p) / Float(res))
    {
        // this will only work for sin_functions
        i = 0;
        std::cout << "Evaluating col: " << j << std::endl;

        // initialize sine function
        GaussianFunction comp_iter_func = comp_func;

        Float mean = (std::pow(p, -c * c) - 1) / (c * c);

        comp_iter_func.solve_for_mean(mean);
        Float max = comp_iter_func.compute_max();

        for (double yx = min_yx; (yx < max_yx) && (i < res); yx += (max_yx - min_yx) / Float(res))
        {
            Float maj = max * yx;

            for (int k = 0; k < techniques.size(); ++k)
                techniques[k]->setMaj(maj);

            PinkDavis recip = PinkDavis(&comp_iter_func, c);

            // initialize a sample interval to evaluate the convergence of
            PowerTwoSampleInterval sample_interval_short = PowerTwoSampleInterval(trials, HeatSamples);

            ConvergenceEvaluatorHeat<Float> evaluator = ConvergenceEvaluatorHeat<Float>(&recip, techniques, &sample_interval_short, rng.nextUInt(), rng.nextUInt());
            evaluator.evaluate();

            // create convergence data
            ConvergenceData avg_data = ConvergenceData(evaluator.getData()->data_records);

            for (int k = 0; k < techniques.size(); ++k)
            {
                int data_pts = avg_data.getRecordByIndex(k).data_points.size();

                images[5 * k + 0]->setPixel(j, i, imedit::Pixel(avg_data.getRecordByIndex(k).data_points[data_pts - 1].mean));
                images[5 * k + 1]->setPixel(j, i, imedit::Pixel(avg_data.getRecordByIndex(k).data_points[data_pts - 1].bias));
                images[5 * k + 2]->setPixel(j, i, imedit::Pixel(avg_data.getRecordByIndex(k).data_points[data_pts - 1].true_mse));
                images[5 * k + 3]->setPixel(j, i, imedit::Pixel(avg_data.getRecordByIndex(k).data_points[data_pts - 1].work));
                images[5 * k + 4]->setPixel(j, i, imedit::Pixel(avg_data.getRecordByIndex(k).data_points[data_pts - 1].work_norm_var));
            }

            mean_image.setPixel(j, i, imedit::Pixel(recip.eval(comp_iter_func.integrate())));

            i++;
        }
        j++;
    }

    for (int k = 0; k < techniques.size(); ++k)
    {
        std::cout << "path: " << getDataPath() + prefix + techniques[k]->getName() + "_mean.exr" << std::endl;
        images[5 * k + 0]->write(getDataPath() + prefix + techniques[k]->getName() + "_mean.exr");
        images[5 * k + 1]->write(getDataPath() + prefix + techniques[k]->getName() + "_err.exr");
        images[5 * k + 2]->write(getDataPath() + prefix + techniques[k]->getName() + "_mse.exr");
        images[5 * k + 3]->write(getDataPath() + prefix + techniques[k]->getName() + "_work.exr");
        images[5 * k + 4]->write(getDataPath() + prefix + techniques[k]->getName() + "_work_norm_var.exr");
    }

    mean_image.write(getDataPath() + prefix + "gt_mean.exr");
}

void PinkDavisHeatPlots::run() const
{
    Float c = 0.8;
    PCumePink<Float> *pcume = new PCumePink<Float>(1.0, c);
    PCumePinkExp<Float> *pcume_exp = new PCumePinkExp<Float>(1.0, c);
    PCmfPinkExp<Float> *pcmf_exp = new PCmfPinkExp<Float>(1.0, c);
    RatioPink<Float> *rat = new RatioPink<Float>(1.0, c);
    TelescopingFinal<Float> *tele_zero = new TelescopingFinal<Float>(0.65, 0);
    TelescopingFinal<Float> *tele_one = new TelescopingFinal<Float>(0.65, 1);
    TelescopingFinal<Float> *tele_two = new TelescopingFinal<Float>(0.65, 2);
    NvidiaPinkExp<Float> *nvidia_exp = new NvidiaPinkExp<Float>(1.0, c);
    NvidiaPink<Float> *nvidia = new NvidiaPink<Float>(1.0, c, 5);

    std::vector<Technique<Float> *> techniques = std::vector<Technique<Float> *>();

    pcg32 rng = pcg32(0x12312, 0xadcd);

    // std::string prefix = "noise_all/";

    // techniques.push_back(tele_zero);
    // techniques.push_back(tele_one);
    // techniques.push_back(tele_two);
    // techniques.push_back(pcume);
    // techniques.push_back(pcume_exp);
    // techniques.push_back(pcmf_exp);
    // techniques.push_back(rat);
    // techniques.push_back(nvidia_exp);
    techniques.push_back(nvidia);

    // run_experiment_const(rng, c, techniques, "const_all/");
    // run_experiment_noise(rng, c, techniques, "noise_all/");
    run_experiment_scos(rng, c, techniques, "scos_all/");
    // run_experiment_gauss(rng, c, techniques, "gauss_all/");

    // finish by cleaning up the datastructures
    delete pcume;
    delete pcume_exp;
    delete pcmf_exp;
    delete rat;
    delete tele_zero;
    delete tele_one;
    delete tele_two;
    delete nvidia_exp;
    delete nvidia;
}

void PinkDavisHeatPlots::run(int test) const
{
    Float c = 0.5;
    PCumePink<Float> *pcume = new PCumePink<Float>(1.0, c);
    RatioPink<Float> *rat = new RatioPink<Float>(1.0, c);
    TelescopingFinal<Float> *tele = new TelescopingFinal<Float>(0.65);

    std::vector<Technique<Float> *> techniques = std::vector<Technique<Float> *>();

    pcg32 rng = pcg32(0x12312, 0xadcd);

    if (test == 0)
    {
        techniques.push_back(tele);
        run_experiment_const(rng, c, techniques, "const_all/");
    }
    if (test == 1)
    {
        techniques.push_back(tele);
        run_experiment_noise(rng, c, techniques, "noise_all/");
    }
    if (test == 2)
    {
        techniques.push_back(tele);
        run_experiment_scos(rng, c, techniques, "scos_all/");
    }
    if (test == 3)
    {
        techniques.push_back(tele);
        run_experiment_gauss(rng, c, techniques, "gauss_all/");
    }
    if (test == 4)
    {
        techniques.push_back(pcume);
        run_experiment_const(rng, c, techniques, "const_all/");
    }
    if (test == 5)
    {
        techniques.push_back(pcume);
        run_experiment_noise(rng, c, techniques, "noise_all/");
    }
    if (test == 6)
    {
        techniques.push_back(pcume);
        run_experiment_scos(rng, c, techniques, "scos_all/");
    }
    if (test == 7)
    {
        techniques.push_back(pcume);
        run_experiment_gauss(rng, c, techniques, "gauss_all/");
    }
    if (test == 8)
    {
        techniques.push_back(rat);
        run_experiment_const(rng, c, techniques, "const_all/");
    }
    if (test == 9)
    {
        techniques.push_back(rat);
        run_experiment_noise(rng, c, techniques, "noise_all/");
    }
    if (test == 10)
    {
        techniques.push_back(rat);
        run_experiment_scos(rng, c, techniques, "scos_all/");
    }
    if (test == 11)
    {
        techniques.push_back(rat);
        run_experiment_gauss(rng, c, techniques, "gauss_all/");
    }

    // finish by cleaning up the datastructures
    delete tele;
    delete pcume;
    delete rat;
}
