#include <conver/conver.h>

struct ListComputation
{
    static Float compute_mean(const std::vector<Float> &vals)
    {
        Float mean = 0.0;

        for (int i = 0; i < vals.size(); ++i)
        {
            mean = mean + 1.0 / Float(i + 1) * (vals[i] - mean);
        }

        return mean;
    }

    static Float compute_abs_diff(Float gt, const std::vector<Float> &vals)
    {
        Float res = 0.0;

        for (int i = 0; i < vals.size(); ++i)
        {
            Float val = std::abs(vals[i] - gt);
            res = res + 1.0 / Float(i + 1) * (val - res);
        }

        return res;
    }

    // TODO: change this name to be less confusing
    // TODO: compute sample mean and mean -> include bessel correction for sample
    // TODO: sample mse, gt mse, absolute sample deviation, absolute gt deviation
    static Float compute_mse(Float gt, const std::vector<Float> &vals)
    {
        Float res = 0.0;

        for (int i = 0; i < vals.size(); ++i)
        {
            Float val = (vals[i] - gt) * (vals[i] - gt);
            res = res + 1.0 / Float(i + 1) * (val - res);
        }

        return res;
    }
};