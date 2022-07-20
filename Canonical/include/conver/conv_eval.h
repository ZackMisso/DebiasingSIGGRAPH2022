#pragma once

#include <conver/conver.h>
#include <conver/problem.h>
#include <conver/conv_data.h>
#include <conver/grapher.h>
#include <conver/sample_interval.h>
#include <conver/technique.h>

template <typename T>
struct ConvergenceEvaluator
{
public:
    ConvergenceEvaluator(Problem<T> *problem,
                         const std::vector<Technique<T> *> &techs,
                         SampleInterval *intervals,
                         uint32_t seed_one = 0x1234,
                         uint32_t seed_two = 0x4562)
        : problem(problem), intervals(intervals), seed_one(seed_one), seed_two(seed_two), data(0x0)
    {
        for (int i = 0; i < techs.size(); ++i)
        {
            techniques.push_back(techs[i]);
        }
    }

    // evaluate may have many different implementations depending on the templated parameter
    void evaluate();

    ConvergenceData *getData() const { return data; }

    void debug() const
    {
        std::cout << "techs: " << techniques.size() << std::endl;
        std::cout << "records: " << data->data_records.size() << std::endl;
    }

protected:
    ConvergenceData *data;
    Problem<T> *problem;
    SampleInterval *intervals;
    std::vector<Technique<T> *> techniques;

    uint32_t seed_one;
    uint32_t seed_two;
};

// // the same as convergence_evaluator, but this estimator runs until a specific
// // cost is reached
// template <typename T>
// struct ConvergenceEvaluatorCostLimit
// {
// public:
//     ConvergenceEvaluatorCostLimit(Problem<T> *problem,
//                                   const std::vector<Technique<T> *> &techs,
//                                   Float max_cost,
//                                   uint32_t seed_one = 0x1234,
//                                   uint32_t seed_two = 0x4562)
//         : problem(problem), intervals(intervals), max_cost(max_cost), seed_one(seed_one), seed_two(seed_two), data(0x0)
//     {
//         for (int i = 0; i < techs.size(); ++i)
//         {
//             techniques.push_back(techs[i]);
//         }
//     }

//     // evaluate may have many different implementations depending on the templated parameter
//     void evaluate();

//     ConvergenceData *getData() const { return data; }

//     // void debug() const
//     // {
//     //     std::cout << "techs: " << techniques.size() << std::endl;
//     //     std::cout << "records: " << data->data_records.size() << std::endl;
//     // }

// protected:
//     ConvergenceData *data;
//     Problem<T> *problem;
//     std::vector<Technique<T> *> techniques;

//     Float max_cost;
//     uint32_t seed_one;
//     uint32_t seed_two;
// };

// this only differs in that it normalizes the cost by both the samples and trials
template <typename T>
struct ConvergenceEvaluatorHeat
{
public:
    ConvergenceEvaluatorHeat(Problem<T> *problem,
                             const std::vector<Technique<T> *> &techs,
                             SampleInterval *intervals,
                             uint32_t seed_one = 0x1234,
                             uint32_t seed_two = 0x4562)
        : problem(problem), intervals(intervals), seed_one(seed_one), seed_two(seed_two), data(0x0)
    {
        for (int i = 0; i < techs.size(); ++i)
        {
            techniques.push_back(techs[i]);
        }
    }

    // evaluate may have many different implementations depending on the templated parameter
    void evaluate();

    ConvergenceData *getData() const { return data; }

    void debug() const
    {
        std::cout << "techs: " << techniques.size() << std::endl;
        std::cout << "records: " << data->data_records.size() << std::endl;
    }

protected:
    ConvergenceData *data;
    Problem<T> *problem;
    SampleInterval *intervals;
    std::vector<Technique<T> *> techniques;

    uint32_t seed_one;
    uint32_t seed_two;
};

// This is going to be used as a separate class for now, but its functionality
// may get worked into the original soonish
template <typename T>
struct ConvergenceIntegralEvaluator
{
public:
    ConvergenceIntegralEvaluator(Problem<T> *problem,
                                 const std::vector<Technique<T> *> &techs,
                                 SampleInterval *intervals,
                                 uint32_t seed_one = 0x1234,
                                 uint32_t seed_two = 0x4562)
        : problem(problem), intervals(intervals), seed_one(seed_one), seed_two(seed_two), data(0x0)
    {
        for (int i = 0; i < techs.size(); ++i)
        {
            techniques.push_back(techs[i]);
        }
    }

    // evaluate may have many different implementations depending on the templated parameter
    void evaluate();

    ConvergenceData *getData() const { return data; }

    void debug() const
    {
        std::cout << "techs: " << techniques.size() << std::endl;
        std::cout << "records: " << data->data_records.size() << std::endl;
    }

protected:
    ConvergenceData *data;
    Problem<T> *problem;
    SampleInterval *intervals;
    std::vector<Technique<T> *> techniques;

    uint32_t seed_one;
    uint32_t seed_two;
};

// this convergence evaluator only evaluates a single sample, but over a varying number
// of trials. This evaluator should display bias for biased methods and unbiased behavior
// for unbiased methods.
template <typename T>
struct TrialConvergenceEvaluator
{
public:
    TrialConvergenceEvaluator(Problem<T> *problem,
                              const std::vector<Technique<T> *> &techs,
                              SampleInterval *intervals,
                              uint32_t seed_one = 0x1234,
                              uint32_t seed_two = 0x4562)
        : problem(problem), intervals(intervals), seed_one(seed_one), seed_two(seed_two), data(0x0)
    {
        for (int i = 0; i < techs.size(); ++i)
        {
            techniques.push_back(techs[i]);
        }
    }

    // evaluate may have many different implementations depending on the templated parameter
    void evaluate();

    ConvergenceData *getData() const { return data; }

    void debug() const
    {
        std::cout << "techs: " << techniques.size() << std::endl;
        std::cout << "records: " << data->data_records.size() << std::endl;
    }

protected:
    ConvergenceData *data;
    Problem<T> *problem;
    SampleInterval *intervals;
    std::vector<Technique<T> *> techniques;

    uint32_t seed_one;
    uint32_t seed_two;
};