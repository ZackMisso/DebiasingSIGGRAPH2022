#pragma once

#include <conver/conver.h>
#include <conver/experiment.h>

class ExperimentManager
{
public:
    static void run(Experiment *experiment);
    static void run(Experiment *experiment, int test_ind);
    static void runExperiment(int argc, char *argv[]);
};