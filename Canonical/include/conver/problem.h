#pragma once

#include <conver/conver.h>
#include <conver/bounds.h>
#include <conver/simple_funcs.h>

template <typename T>
struct Problem
{
public:
    // these methods evaluate the problem given integral evaluations
    virtual T eval(Float sample) const = 0;
    virtual T eval(const std::vector<Float> &samples) const { return T(0); }
    virtual T eval_function(Float sample) const { return T(0); }
    // these methods just evaluate the integrals with no non-linear modification
    virtual T eval_integral(Float sample) const { return T(0); }
    virtual T eval_integral(const std::vector<Float> &samples) const { return T(0); }
    // this is the expected value of the non-linearly modified integral
    virtual T expected() const = 0;
    // this is the expected value of the integral
    virtual T expected_integral() const { return T(0); }

    virtual T pre_sample(pcg32 &rng) { return T(0); }

    virtual T eval(Float sample, Float min) const
    {
        return eval(sample);
    }

    virtual T eval(const std::vector<Float> &samples, Float min) const
    {
        return eval(samples);
    }

    virtual T eval_rep_if_zero(Float sample, Float min) const
    {
        return eval(sample);
    }

    virtual T eval_rep_if_zero(const std::vector<Float> &samples, Float min) const
    {
        return eval(samples);
    }

    virtual Float eval_rot(Float sample, Float epsilon, int m) const
    {
        return eval(sample);
    }

    virtual Float eval_rot(const std::vector<Float> &samples) const
    {
        return eval(samples);
    }

    virtual T get_variable(int val) const
    {
        std::cout << "ERROR: THIS SHOULD NEVER BE CALLED: non-override get_variable" << std::endl;
        return T(0.0);
    }

    Bounds<T> *getBounds() const { return bounds; }
    virtual std::string getName() const = 0;

protected:
    Bounds<T> *bounds;
};

struct ExpTrans : public Problem<Float>
{
    ExpTrans(Function *func);
    ~ExpTrans();

    virtual Float eval(Float sample) const;
    virtual Float eval(const std::vector<Float> &samples) const;
    virtual Float eval_integral(Float sample) const;
    virtual Float eval_integral(const std::vector<Float> &samples) const;
    virtual Float expected() const;
    virtual Float expected_integral() const;
    virtual Float eval_function(Float sample) const;

    virtual std::string getName() const { return "exp_trans"; }

protected:
    Function *optical_depth;
};

struct PinkDavis : public Problem<Float>
{
    PinkDavis(Function *func, Float c);
    ~PinkDavis();

    virtual Float eval(Float sample) const;
    virtual Float eval(const std::vector<Float> &samples) const;
    virtual Float eval_integral(Float sample) const;
    virtual Float eval_integral(const std::vector<Float> &samples) const;
    virtual Float expected() const;
    virtual Float expected_integral() const;
    virtual Float eval_function(Float sample) const;

    virtual std::string getName() const { return "pink_davis"; }

protected:
    Function *optical_depth;
    Float c;
};

struct Reciprocal : public Problem<Float>
{
    Reciprocal(Function *func);
    ~Reciprocal();

    virtual Float eval(Float sample) const;
    virtual Float eval(const std::vector<Float> &samples) const;
    virtual Float eval(Float sample, Float min) const;
    virtual Float eval(const std::vector<Float> &samples, Float min) const;
    virtual Float eval_rep_if_zero(Float sample, Float min) const;
    virtual Float eval_rep_if_zero(const std::vector<Float> &samples, Float min) const;
    virtual Float eval_rot(Float sample, Float epsilon, int m) const;
    virtual Float eval_rot(const std::vector<Float> &samples) const;
    virtual Float eval_function(Float sample) const;
    virtual Float eval_integral(Float sample) const;
    virtual Float eval_integral(const std::vector<Float> &samples) const;
    virtual Float expected() const;
    virtual Float expected_integral() const;

    virtual std::string getName() const { return "reciprocal"; }

protected:
    Function *func;
};

struct WASSimpleReciprocal : public Reciprocal
{
    WASSimpleReciprocal(Function *func) : Reciprocal(func) {}

    virtual std::string getName() const { return "was_simple"; }
};

struct SimpleFD : public Problem<Float>
{
    SimpleFD(Function *func, Float pt);
    ~SimpleFD();

    virtual Float eval(Float sample) const;
    virtual Float eval(const std::vector<Float> &samples) const;
    virtual Float expected() const;

    virtual std::string getName() const { return "simple_fd"; }

protected:
    Function *func;
    Float pt;
};

struct SimpleAO : public Problem<Float>
{
    SimpleAO(Float sphere_height, Float sphere_radius);
    ~SimpleAO();

    // TODO: maybe update this
    virtual Float eval(Float sample) const;
    virtual Float eval(const std::vector<Float> &samples) const;

    virtual Float expected() const;

    virtual std::string getName() const { return "simple_ao"; }

protected:
    Float sphere_height;
    Float sphere_radius;
};

struct PowerLaw : public Problem<Float>
{
    PowerLaw(Function *func, Float alpha);
    ~PowerLaw();

    virtual Float eval(Float sample) const;
    virtual Float eval(const std::vector<Float> &samples) const;
    virtual Float eval_integral(Float sample) const;
    virtual Float eval_integral(const std::vector<Float> &samples) const;
    virtual Float expected() const;
    virtual Float expected_integral() const;
    virtual Float eval_function(Float sample) const;

    virtual std::string getName() const { return "pow_law_" + std::to_string(alpha); }

protected:
    Function *optical_depth; // TODO: change the name of this eventually
    Float alpha;
};

struct IntegralEval : public Problem<Float>
{
    IntegralEval(Function *func);
    ~IntegralEval();

    virtual Float eval(Float sample) const;
    virtual Float eval(const std::vector<Float> &samples) const;
    virtual Float eval_integral(Float sample) const;
    virtual Float eval_integral(const std::vector<Float> &samples) const;
    virtual Float expected() const;
    virtual Float expected_integral() const;
    virtual Float eval_function(Float sample) const;

    virtual std::string getName() const { return "integral"; }

protected:
    Function *optical_depth; // TODO: change the name of this eventually
    Float alpha;
};

struct Step : public Problem<Float>
{
    Step(Function *func, Float x, Float val_1, Float val_2);
    ~Step();

    virtual Float eval(Float sample) const;
    virtual Float eval(const std::vector<Float> &samples) const;
    virtual Float eval_integral(Float sample) const;
    virtual Float eval_integral(const std::vector<Float> &samples) const;
    virtual Float expected() const;
    virtual Float expected_integral() const;
    virtual Float eval_function(Float sample) const;

    virtual std::string getName() const { return "integral"; }

protected:
    Function *func; // TODO: change the name of this eventually
    Float x;
    Float val_1;
    Float val_2;
};

struct StepIntegral : public Problem<Float>
{
    StepIntegral(Function *func,
                 Float x,
                 Float val_1,
                 Float val_2,
                 Float min_x,
                 Float max_x);
    ~StepIntegral();

    virtual Float eval(Float sample) const;
    virtual Float eval(const std::vector<Float> &samples) const;
    virtual Float eval_integral(Float sample) const;
    virtual Float eval_integral(const std::vector<Float> &samples) const;
    virtual Float expected() const;
    virtual Float expected_integral() const;
    virtual Float eval_function(Float sample) const;

    virtual std::string getName() const { return "integral"; }

    virtual Float pre_sample(pcg32 &rng)
    {
        Float mean = rng.nextDouble() * (max_x - min_x) + min_x;
        func->solve_for_mean(mean);
        return (max_x - min_x);
    }

protected:
    Function *func; // TODO: change the name of this eventually
    Float x;
    Float val_1;
    Float val_2;
    Float min_x;
    Float max_x;
};

struct Staircase : public Problem<Float>
{
    Staircase(Function *func,
              Float x_step,
              Float y_step,
              Float x_start,
              Float y_start);
    ~Staircase();

    virtual Float eval(Float sample) const;
    virtual Float eval(const std::vector<Float> &samples) const;
    virtual Float eval_integral(Float sample) const;
    virtual Float eval_integral(const std::vector<Float> &samples) const;
    virtual Float expected() const;
    virtual Float expected_integral() const;
    virtual Float eval_function(Float sample) const;

    virtual std::string getName() const { return "integral"; }

protected:
    Function *func; // TODO: change the name of this eventually
    Float x_step;
    Float y_step;
    Float x_start; // maybe remove x_start
    Float y_start;
};

struct PhotonProblem : public Problem<Float>
{
    PhotonProblem(Float emitter_z = 1.0,
                  Float emitter_power = 10.0)
        : emitter_z(emitter_z),
          emitter_power(emitter_power) {}
    ~PhotonProblem() {}

    virtual Float eval(Float sample) const
    {
        return sample;
    }

    virtual Float eval(const std::vector<Float> &samples) const
    {
        return samples[0];
    }
    virtual Float eval_integral(Float sample) const { return 0.0; }
    virtual Float eval_integral(const std::vector<Float> &samples) const { return 0.0; }
    virtual Float expected() const { return emitter_power / (2.0 * M_PI * emitter_z * emitter_z); }
    virtual Float expected_integral() const { return 0.0; }
    virtual Float eval_function(Float sample) const { return 0.0; }

    virtual Float get_variable(int val) const
    {
        if (val == PHOTON_EMITTER_POWER)
            return emitter_power;
        if (val == PHOTON_EMITTER_Z)
            return emitter_z;
        return 0.0;
    }

    virtual std::string getName() const { return "photon_density"; }

    Float emitter_z;
    Float emitter_power;
};