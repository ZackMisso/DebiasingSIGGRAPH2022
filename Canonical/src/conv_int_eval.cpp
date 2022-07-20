#include <conver/conv_eval.h>
#include <conver/list_comp.h>

template <>
void ConvergenceIntegralEvaluator<Float>::evaluate()
{
    pcg32 rng = pcg32(seed_one, seed_two);
    Float expected = problem->expected();

    if (data)
        delete data;

    data = new ConvergenceData();

    for (int i = 0; i < techniques.size(); ++i)
    {
        intervals->start();

        ConvergenceRecord record = ConvergenceRecord(techniques[i]->getName());

        double val;
        int samples;

        while (intervals->getNextInterval(val, samples))
        {
            std::cout << "evaluating secondary estimator for samples: " << samples << std::endl;
            // std::cout << "Expected: " << expected << std::endl;
            int trials = intervals->getTrials();

            DataPoint data_point = DataPoint();

            std::vector<Float> evals = std::vector<Float>(trials);
            Float cost = 0.0;

            for (int j = 0; j < trials; ++j)
            {
                evals[j] = techniques[i]->eval_integral_problem_secondary(problem, samples, cost, rng);
            }

            Float true_rmse = 0.0;
            Float sample_rmse = 0.0;
            Float sample_mean = 0.0;
            Float avg_cost = cost / Float(trials);

            // TODO: compute all the things
            sample_mean = ListComputation::compute_mean(evals);
            data_point.sample_abs_diff = ListComputation::compute_abs_diff(sample_mean, evals);
            data_point.true_abs_diff = ListComputation::compute_abs_diff(expected, evals);
            data_point.sample_mse = ListComputation::compute_mse(sample_mean, evals);
            data_point.true_mse = ListComputation::compute_mse(expected, evals);

            data_point.samples = samples;
            data_point.rmse = std::sqrt(data_point.true_mse);
            data_point.mean = sample_mean;
            data_point.bias = abs(sample_mean - expected);
            data_point.work = avg_cost;
            // TODO: maybe also store different efficiencies
            data_point.work_norm_var = avg_cost * data_point.rmse;

            record.add_datapoint(data_point);
        }

        data->addRecord(record);
    }
}