#include <conver/conv_eval.h>
#include <conver/list_comp.h>

template <>
void TrialConvergenceEvaluator<Float>::evaluate()
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
            int trials = samples;

            DataPoint data_point = DataPoint();

            std::vector<Float> evals = std::vector<Float>(trials);

            long cost = 0;
            for (int j = 0; j < trials; ++j)
            {
                // this convergence evaluator will always only evaluate one sample
                int tmp = 0;
                evals[j] = techniques[i]->eval_problem(problem, 1, tmp, rng);
                cost += tmp;
            }

            Float true_rmse = 0.0;
            Float sample_rmse = 0.0;
            Float sample_mean = 0.0;
            Float average_cost = Float(cost) / Float(trials);
            Float cost_full = Float(cost);

            // TODO: maybe update this with the correct abs diff comps
            sample_mean = ListComputation::compute_mean(evals);
            sample_rmse = ListComputation::compute_mse(sample_mean, evals);
            true_rmse = ListComputation::compute_mse(expected, evals);

            data_point.samples = samples;
            data_point.rmse = sample_rmse;
            data_point.mean = sample_mean;
            data_point.work = average_cost;
            data_point.bias = abs(sample_mean - expected);
            data_point.work_norm_var = average_cost * sample_rmse;
            data_point.work_norm_err = average_cost * data_point.bias;

            record.add_datapoint(data_point);
        }

        data->addRecord(record);
    }
}