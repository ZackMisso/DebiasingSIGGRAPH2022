#include <conver/simple_funcs.h>

struct SimpleBinaryFunc : public Function
{
    SimpleBinaryFunc(Float a, Float b, Float p) : Function(a, b), p(p) {}

    virtual Float eval(Float x) const
    {
        if (x < p * (b - a))
            return 1.0;
        return 0.0;
    }

    virtual Float integrate() const
    {
        return p * (b - a);
    }

    virtual std::string name() const { return "simple_binary"; }

    Float p;
};

struct RandBinaryFunc : public Function
{
    RandBinaryFunc(Float a, Float b, Float p, int bins, int rands, pcg32 &rng) : Function(a, b), p(p)
    {
        for (int i = 0; i < bins; ++i)
        {
            if (Float(i) / Float(bins) < p)
                vals.push_back(1.0);
            else
                vals.push_back(0.0);
        }

        for (int i = 0; i < rands; ++i)
        {
            int ind_one = rng.nextDouble() * bins;
            int ind_two = rng.nextDouble() * bins;

            Float tmp_val = vals[ind_one];
            vals[ind_one] = vals[ind_two];
            vals[ind_two] = tmp_val;
        }
    }

    virtual Float eval(Float x) const
    {
        int index = floor(x * vals.size());
        return vals[index];
    }

    virtual Float integrate() const
    {
        return p * (b - a);
    }

    virtual std::string name() const { return "rand_binary"; }

    std::vector<Float> vals;
    Float p;
};