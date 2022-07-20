#pragma once

#include <conver/conver.h>
#include <conver/technique.h>

struct DataPoint
{
    DataPoint()
        : samples(0),
          trials(0),
          id(-1),
          variance(0.0),
          work(0.0),
          work_norm_var(0.0),
          work_norm_err(0.0),
          mean(0.0), mse(0.0),
          rmse(0.0),
          bias(0.0),
          sample_mse(0.0),
          true_mse(0.0),
          sample_abs_diff(0.0),
          true_abs_diff(0.0) {}

    // TODO: add work_norm_err into this maybe
    DataPoint(int samples, int trials, int id, Float variance, Float work, Float work_norm_var)
        : samples(samples), trials(trials), id(id), variance(variance), work(work), work_norm_var(work_norm_var) {}

    DataPoint(int samples, int trials, int id, Float mean, Float mse, Float rmse, Float bias)
        : samples(samples), trials(trials), id(id), mean(mean), mse(mse), rmse(rmse), bias(bias) {}

    DataPoint(const DataPoint &other)
        : samples(other.samples),
          trials(other.trials),
          id(other.id),
          variance(other.variance),
          work(other.work),
          work_norm_var(other.work_norm_var),
          work_norm_err(other.work_norm_err),
          mse(other.mse),
          rmse(other.rmse),
          bias(other.bias),
          mean(other.mean),
          sample_mse(other.sample_mse),
          true_mse(other.true_mse),
          sample_abs_diff(other.sample_abs_diff),
          true_abs_diff(other.true_abs_diff) {}

    int samples;
    int trials;
    int id; // specifies which experiment is getting evaluated

    Float variance;
    Float work;
    Float work_norm_var;
    Float work_norm_err;

    Float mean;
    Float bias;
    Float rmse;
    Float mse;

    Float sample_mse;
    Float true_mse;
    Float sample_abs_diff;
    Float true_abs_diff;
};

struct ConvergenceRecord
{
    ConvergenceRecord();
    ConvergenceRecord(std::string tech_name);
    ConvergenceRecord(const ConvergenceRecord &other);

    void add_datapoint(DataPoint &point);
    void add(const ConvergenceRecord &other);
    void normalize(int n);

    std::vector<DataPoint> data_points;
    std::string tech_name;
};

struct ConvergenceData
{
public:
    ConvergenceData();
    ConvergenceData(int num_techniques);
    ConvergenceData(const std::vector<ConvergenceRecord> &records);
    // ConvergenceData(const ConvergenceData &other);

    void saveDataToFile(std::string path) const;
    void readDataFromFile(std::string path);

    void addTechnique(std::string tech_name);
    void addRecord(ConvergenceRecord &record);

    void setTechniqueNameByIndex(std::string name, int index);

    ConvergenceRecord &getRecordByIndex(int index);
    bool getRecordByTechnique(std::string name, ConvergenceRecord &record);

    // protected:
    std::vector<ConvergenceRecord> data_records;
};

// get many different asymptotic rates for ray-marching different funcs
// double check unbiased estimation of the reciprocal mean for non-negative random variables
// weight windows ??? - maybe
// make figure for ao
// write update for things that have been done and things that I still need to do
