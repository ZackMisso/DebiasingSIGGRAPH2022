#pragma once

#include <conver/conver.h>
#include <conver/simple_funcs.h>
#include <conver/conv_data.h>

struct GraphSettings
{
    GraphSettings()
    {
        dirty = false;
        x_log = false;
        y_log = false;
        conv_lines = false;
        x_axis_title = "";
        y_axis_title = "";
        title = "";
    }

    std::string compileArguements();

    bool x_log;
    bool y_log;
    bool conv_lines;
    std::string title;
    std::string x_axis_title;
    std::string y_axis_title;
    bool dirty; // if custom settings should be applied
};

struct Grapher
{
public:
    // this is just going to contain a whole bunch of class methods to automatically graph results

    static void graphFunction(Function &function,
                              std::string path = "../results/test.png",
                              GraphSettings settings = GraphSettings());
    static void graphFunctionFromAToB(Function &function, Float a, Float b, std::string path = "../results/test.png", GraphSettings settings = GraphSettings());
    static void graphRecordRMSE(ConvergenceRecord &record, std::string path = "../results/test.png", GraphSettings settings = GraphSettings());
    static void graphRecordBias(ConvergenceRecord &record, std::string path = "../results/test.png", GraphSettings settings = GraphSettings());
    static void graphRecordMean(ConvergenceRecord &record, std::string path = "../results/test.png", GraphSettings settings = GraphSettings());
    static void graphRecordCost(ConvergenceRecord &record, std::string path = "../results/cost.png", GraphSettings settings = GraphSettings());
    static void graphVectors(const std::vector<Float> &xs,
                             const std::vector<Float> &evals,
                             std::string path,
                             GraphSettings settings);
    static void graphRecordMeans(std::vector<int> &inds,
                                 std::vector<std::string> &names,
                                 ConvergenceData *record,
                                 std::string path = "../results/test.png",
                                 GraphSettings settings = GraphSettings(),
                                 std::string prefix = "");
    static void graphRecordBiases(std::vector<int> &inds,
                                  std::vector<std::string> &names,
                                  ConvergenceData *record,
                                  std::string path = "../results/test.png",
                                  GraphSettings settings = GraphSettings(),
                                  std::string prefix = "");
    static void graphRecordCosts(std::vector<int> &inds,
                                 std::vector<std::string> &names,
                                 ConvergenceData *record,
                                 std::string path = "../results/test.png",
                                 GraphSettings settings = GraphSettings(),
                                 std::string prefix = "");
    static void graphRecordRMSEs(std::vector<int> &inds,
                                 std::vector<std::string> &names,
                                 ConvergenceData *record,
                                 std::string path = "../results/test.png",
                                 GraphSettings settings = GraphSettings(),
                                 std::string prefix = "");
    static void graphRecordEFFs(std::vector<int> &inds,
                                std::vector<std::string> &names,
                                ConvergenceData *record,
                                std::string path = "../results/test.png",
                                GraphSettings settings = GraphSettings(),
                                std::string prefix = "");
    static void graphRecordBestEFFs(std::vector<int> &inds,
                                    std::vector<std::string> &names,
                                    std::vector<ConvergenceData> &records,
                                    std::vector<double> &vals,
                                    std::string path = "../results/test.png",
                                    GraphSettings settings = GraphSettings(),
                                    std::string prefix = "");
    static void graphRecordBestCosts(std::vector<int> &inds,
                                     std::vector<std::string> &names,
                                     std::vector<ConvergenceData> &records,
                                     std::vector<double> &vals,
                                     std::string path = "../results/test.png",
                                     GraphSettings settings = GraphSettings(),
                                     std::string prefix = "");
    static void graphRecordBestBiases(std::vector<int> &inds,
                                      std::vector<std::string> &names,
                                      std::vector<ConvergenceData> &records,
                                      std::vector<double> &vals,
                                      std::string path = "../results/test.png",
                                      GraphSettings settings = GraphSettings(),
                                      std::string prefix = "");
    static void graphRecordBestMeans(std::vector<int> &inds,
                                     std::vector<std::string> &names,
                                     std::vector<ConvergenceData> &records,
                                     std::vector<double> &vals,
                                     std::string path = "../results/test.png",
                                     GraphSettings settings = GraphSettings(),
                                     std::string prefix = "");
    static void graphRecordBestMSEs(std::vector<int> &inds,
                                    std::vector<std::string> &names,
                                    std::vector<ConvergenceData> &records,
                                    std::vector<double> &vals,
                                    std::string path = "../results/test.png",
                                    GraphSettings settings = GraphSettings(),
                                    std::string prefix = "");
    static void graphRecordSampAbsErrors(std::vector<int> &inds,
                                         std::vector<std::string> &names,
                                         ConvergenceData *record,
                                         std::string path = "../results/test.png",
                                         GraphSettings settings = GraphSettings(),
                                         std::string prefix = "");
    static void graphRecordTrueAbsErrors(std::vector<int> &inds,
                                         std::vector<std::string> &names,
                                         ConvergenceData *record,
                                         std::string path = "../results/test.png",
                                         GraphSettings settings = GraphSettings(),
                                         std::string prefix = "");
    static void graphRecordSampMSEs(std::vector<int> &inds,
                                    std::vector<std::string> &names,
                                    ConvergenceData *record,
                                    std::string path = "../results/test.png",
                                    GraphSettings settings = GraphSettings(),
                                    std::string prefix = "");
    static void graphRecordTrueMSEs(std::vector<int> &inds,
                                    std::vector<std::string> &names,
                                    ConvergenceData *record,
                                    std::string path = "../results/test.png",
                                    GraphSettings settings = GraphSettings(),
                                    std::string prefix = "");

protected:
};