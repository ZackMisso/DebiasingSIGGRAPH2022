#include <conver/grapher.h>

std::string GraphSettings::compileArguements()
{
    std::string args = "";

    if (!title.empty())
        args = args + "--title " + title + " k ";
    if (!x_axis_title.empty())
        args = args + "--x_title " + x_axis_title + " k ";
    if (!y_axis_title.empty())
        args = args + "--y_title " + y_axis_title + " k ";
    if (x_log)
        args = args + "--x_log ";
    if (y_log)
        args = args + "--y_log ";
    if (conv_lines)
        args = args + "--conv_lines ";

    return args;
}

void Grapher::graphFunction(Function &function, std::string path, GraphSettings settings)
{
    std::vector<Float> xs = std::vector<Float>();
    std::vector<Float> evals = std::vector<Float>();

    for (Float x = function.a; x <= function.b; x += (function.b - function.a) / 100000.0)
    {
        xs.push_back(x);
        evals.push_back(function.eval(x));
    }

    std::ofstream temp_file;
    temp_file.open("../results/temp.txt");

    for (int i = 0; i < evals.size(); ++i)
    {
        temp_file << xs[i] << std::endl;
        temp_file << evals[i] << std::endl;
    }

    temp_file.close();

    std::string args = settings.compileArguements();

    std::string cmd = "python3 ../scripts/graph_func.py ../results/temp.txt " + path + " " + args;
    system(cmd.c_str());
}

void Grapher::graphFunctionFromAToB(Function &function, Float a, Float b, std::string path, GraphSettings settings)
{
    std::vector<Float> xs = std::vector<Float>();
    std::vector<Float> evals = std::vector<Float>();

    for (Float x = a; x <= b; x += (b - a) / 1000.0)
    {
        xs.push_back(x);
        evals.push_back(function.eval(x));
    }

    std::ofstream temp_file;
    temp_file.open("../results/temp.txt");

    for (int i = 0; i < evals.size(); ++i)
    {
        temp_file << xs[i] << std::endl;
        temp_file << evals[i] << std::endl;
    }

    temp_file.close();

    std::string args = settings.compileArguements();

    std::string cmd = "python3 ../scripts/graph_func.py ../results/temp.txt " + path + " " + args;
    system(cmd.c_str());
}

void Grapher::graphVectors(const std::vector<Float> &xs,
                           const std::vector<Float> &evals,
                           std::string path,
                           GraphSettings settings)
{
    std::ofstream temp_file;
    temp_file.open("../results/temp.txt");

    for (int i = 0; i < evals.size(); ++i)
    {
        temp_file << xs[i] << std::endl;
        temp_file << evals[i] << std::endl;
    }

    temp_file.close();

    std::string args = settings.compileArguements();

    std::string cmd = "python3 ../scripts/graph_func.py ../results/temp.txt " + path + " " + args;
    system(cmd.c_str());
}

void Grapher::graphRecordRMSE(ConvergenceRecord &record, std::string path, GraphSettings settings)
{
    std::vector<Float> xs = std::vector<Float>();
    std::vector<Float> evals = std::vector<Float>();

    for (int i = 0; i < record.data_points.size(); i++)
    {
        xs.push_back(record.data_points[i].samples);
        evals.push_back(record.data_points[i].rmse);
    }

    std::ofstream temp_file;
    temp_file.open("../results/temp.txt");

    for (int i = 0; i < evals.size(); ++i)
    {
        temp_file << xs[i] << std::endl;
        temp_file << evals[i] << std::endl;
    }

    temp_file.close();

    std::string args = settings.compileArguements();

    std::string cmd = "python3 ../scripts/graph_func.py ../results/temp.txt " + path + " " + args;
    system(cmd.c_str());

    // throw new NotImplementedException();
}

void Grapher::graphRecordBias(ConvergenceRecord &record, std::string path, GraphSettings settings)
{
    std::vector<Float> xs = std::vector<Float>();
    std::vector<Float> evals = std::vector<Float>();

    for (int i = 0; i < record.data_points.size(); i++)
    {
        xs.push_back(record.data_points[i].samples);
        evals.push_back(record.data_points[i].bias);
    }

    std::ofstream temp_file;
    temp_file.open("../results/temp.txt");

    for (int i = 0; i < evals.size(); ++i)
    {
        temp_file << xs[i] << std::endl;
        temp_file << evals[i] << std::endl;
    }

    temp_file.close();

    // settings.x_log = true;
    // settings.y_log = true;
    std::string args = settings.compileArguements();

    std::string cmd = "python3 ../scripts/graph_func.py ../results/temp.txt " + path + " " + args;
    system(cmd.c_str());
}

void Grapher::graphRecordCost(ConvergenceRecord &record, std::string path, GraphSettings settings)
{
    std::vector<Float> xs = std::vector<Float>();
    std::vector<Float> evals = std::vector<Float>();

    for (int i = 0; i < record.data_points.size(); i++)
    {
        xs.push_back(record.data_points[i].samples);
        evals.push_back(record.data_points[i].work);
    }

    std::ofstream temp_file;
    temp_file.open("../results/temp.txt");

    for (int i = 0; i < evals.size(); ++i)
    {
        temp_file << xs[i] << std::endl;
        temp_file << evals[i] << std::endl;
    }

    temp_file.close();

    std::string args = settings.compileArguements();

    std::string cmd = "python3 ../scripts/graph_func.py ../results/temp.txt " + path + " " + args;
    system(cmd.c_str());
}

void Grapher::graphRecordMean(ConvergenceRecord &record, std::string path, GraphSettings settings)
{
    std::vector<Float> xs = std::vector<Float>();
    std::vector<Float> evals = std::vector<Float>();

    for (int i = 0; i < record.data_points.size(); i++)
    {
        xs.push_back(record.data_points[i].samples);
        evals.push_back(record.data_points[i].mean);
    }

    std::ofstream temp_file;
    temp_file.open("../results/temp.txt");

    for (int i = 0; i < evals.size(); ++i)
    {
        temp_file << xs[i] << std::endl;
        temp_file << evals[i] << std::endl;
    }

    temp_file.close();

    std::string args = settings.compileArguements();

    std::string cmd = "python3 ../scripts/graph_func.py ../results/temp.txt " + path + " " + args;
    system(cmd.c_str());
}

void Grapher::graphRecordMeans(std::vector<int> &inds,
                               std::vector<std::string> &names,
                               ConvergenceData *records,
                               std::string path,
                               GraphSettings settings,
                               std::string prefix)
{
    std::string files = "";

    for (int k = 0; k < inds.size(); ++k)
    {
        ConvergenceRecord record = records->getRecordByIndex(inds[k]);

        std::vector<Float> xs = std::vector<Float>();
        std::vector<Float> evals = std::vector<Float>();

        for (int i = 0; i < record.data_points.size(); i++)
        {
            xs.push_back(record.data_points[i].samples);
            evals.push_back(record.data_points[i].mean);
        }

        std::ofstream temp_file;
        temp_file.open("../results/" + prefix + "temp_means_" + std::to_string(inds[k]) + ".txt");

        files += "../results/" + prefix + "temp_means_" + std::to_string(inds[k]) + ".txt " + names[k] + " ";

        for (int i = 0; i < evals.size(); ++i)
        {
            temp_file << xs[i] << std::endl;
            temp_file << evals[i] << std::endl;
        }

        temp_file.close();
    }

    std::string args = settings.compileArguements();

    std::string cmd = "python3 ../scripts/graph_funcs_tmp.py " + std::to_string(inds.size()) + " " + files + path + " " + args;
    system(cmd.c_str());
}

void Grapher::graphRecordBiases(std::vector<int> &inds,
                                std::vector<std::string> &names,
                                ConvergenceData *records,
                                std::string path,
                                GraphSettings settings,
                                std::string prefix)
{
    std::string files = "";

    for (int k = 0; k < inds.size(); ++k)
    {
        ConvergenceRecord record = records->getRecordByIndex(inds[k]);

        std::vector<Float> xs = std::vector<Float>();
        std::vector<Float> evals = std::vector<Float>();

        for (int i = 0; i < record.data_points.size(); i++)
        {
            xs.push_back(record.data_points[i].samples);
            evals.push_back(record.data_points[i].bias);
        }

        std::ofstream temp_file;
        temp_file.open("../results/" + prefix + "temp_errs_" + std::to_string(inds[k]) + ".txt");

        files += "../results/" + prefix + "temp_errs_" + std::to_string(inds[k]) + ".txt " + names[k] + " ";

        for (int i = 0; i < evals.size(); ++i)
        {
            temp_file << xs[i] << std::endl;
            temp_file << evals[i] << std::endl;
        }

        temp_file.close();
    }

    std::string args = settings.compileArguements();
    std::cout << "ARGS: " << args << std::endl;

    std::string cmd = "python3 ../scripts/graph_funcs_tmp.py " + std::to_string(inds.size()) + " " + files + path + " " + args;
    system(cmd.c_str());
}

void Grapher::graphRecordRMSEs(std::vector<int> &inds,
                               std::vector<std::string> &names,
                               ConvergenceData *records,
                               std::string path,
                               GraphSettings settings,
                               std::string prefix)
{
    std::string files = "";

    for (int k = 0; k < inds.size(); ++k)
    {
        ConvergenceRecord record = records->getRecordByIndex(inds[k]);

        std::vector<Float> xs = std::vector<Float>();
        std::vector<Float> evals = std::vector<Float>();

        for (int i = 0; i < record.data_points.size(); i++)
        {
            xs.push_back(record.data_points[i].samples);
            evals.push_back(record.data_points[i].rmse);
        }

        std::ofstream temp_file;
        temp_file.open("../results/" + prefix + "temp_mses_" + std::to_string(inds[k]) + ".txt");

        files += "../results/" + prefix + "temp_mses_" + std::to_string(inds[k]) + ".txt " + names[k] + " ";

        for (int i = 0; i < evals.size(); ++i)
        {
            temp_file << xs[i] << std::endl;
            temp_file << evals[i] << std::endl;
        }

        temp_file.close();
    }

    std::string args = settings.compileArguements();
    std::cout << "ARGS: " << args << std::endl;

    std::string cmd = "python3 ../scripts/graph_funcs_tmp.py " + std::to_string(inds.size()) + " " + files + path + " " + args;
    system(cmd.c_str());
}

void Grapher::graphRecordCosts(std::vector<int> &inds,
                               std::vector<std::string> &names,
                               ConvergenceData *records,
                               std::string path,
                               GraphSettings settings,
                               std::string prefix)
{
    std::string files = "";

    for (int k = 0; k < inds.size(); ++k)
    {
        ConvergenceRecord record = records->getRecordByIndex(inds[k]);

        std::vector<Float> xs = std::vector<Float>();
        std::vector<Float> evals = std::vector<Float>();

        for (int i = 0; i < record.data_points.size(); i++)
        {
            xs.push_back(record.data_points[i].samples);
            evals.push_back(record.data_points[i].work);
        }

        std::ofstream temp_file;
        temp_file.open("../results/" + prefix + "temp_costs_" + std::to_string(inds[k]) + ".txt");

        files += "../results/" + prefix + "temp_costs_" + std::to_string(inds[k]) + ".txt " + names[k] + " ";

        for (int i = 0; i < evals.size(); ++i)
        {
            temp_file << xs[i] << std::endl;
            temp_file << evals[i] << std::endl;
        }

        temp_file.close();
    }

    std::string args = settings.compileArguements();

    std::string cmd = "python3 ../scripts/graph_funcs_tmp.py " + std::to_string(inds.size()) + " " + files + path + " " + args;
    system(cmd.c_str());
}

void Grapher::graphRecordEFFs(std::vector<int> &inds,
                              std::vector<std::string> &names,
                              ConvergenceData *records,
                              std::string path,
                              GraphSettings settings,
                              std::string prefix)
{
    std::string files = "";

    for (int k = 0; k < inds.size(); ++k)
    {
        ConvergenceRecord record = records->getRecordByIndex(inds[k]);

        std::vector<Float> xs = std::vector<Float>();
        std::vector<Float> evals = std::vector<Float>();

        for (int i = 0; i < record.data_points.size(); i++)
        {
            xs.push_back(record.data_points[i].samples);
            evals.push_back(record.data_points[i].work_norm_var);
        }

        std::ofstream temp_file;
        temp_file.open("../results/" + prefix + "temp_effs_" + std::to_string(inds[k]) + ".txt");
        std::cout << "HERE: "
                  << "../results/" + prefix + "temp_effs_" + std::to_string(inds[k]) + ".txt" << std::endl;
        files += "../results/" + prefix + "temp_effs_" + std::to_string(inds[k]) + ".txt " + names[k] + " ";

        for (int i = 0; i < evals.size(); ++i)
        {
            temp_file << xs[i] << std::endl;
            temp_file << evals[i] << std::endl;
        }

        temp_file.close();
    }

    std::string args = settings.compileArguements();

    std::string cmd = "python3 ../scripts/graph_funcs_tmp.py " + std::to_string(inds.size()) + " " + files + path + " " + args;
    system(cmd.c_str());
}

void Grapher::graphRecordBestEFFs(std::vector<int> &inds,
                                  std::vector<std::string> &names,
                                  std::vector<ConvergenceData> &records,
                                  std::vector<double> &vals,
                                  std::string path,
                                  GraphSettings settings,
                                  std::string prefix)
{
    std::string files = "";

    for (int k = 0; k < inds.size(); ++k)
    {
        std::vector<Float> evals = std::vector<Float>();

        for (int i = 0; i < records.size(); i++)
        {
            int data_pts = records[i].getRecordByIndex(inds[k]).data_points.size();
            evals.push_back(records[i].getRecordByIndex(inds[k]).data_points[data_pts - 1].work_norm_var);
        }

        std::ofstream temp_file;
        temp_file.open("../results/" + prefix + "temp_effs_" + std::to_string(inds[k]) + ".txt");
        files += "../results/" + prefix + "temp_effs_" + std::to_string(inds[k]) + ".txt " + names[k] + " ";

        for (int i = 0; i < evals.size(); ++i)
        {
            temp_file << vals[i] << std::endl;
            temp_file << evals[i] << std::endl;
        }

        temp_file.close();
    }

    std::string args = settings.compileArguements();

    std::string cmd = "python3 ../scripts/graph_funcs_tmp.py " + std::to_string(inds.size()) + " " + files + path + " " + args;
    system(cmd.c_str());
}

void Grapher::graphRecordBestCosts(std::vector<int> &inds,
                                   std::vector<std::string> &names,
                                   std::vector<ConvergenceData> &records,
                                   std::vector<double> &vals,
                                   std::string path,
                                   GraphSettings settings,
                                   std::string prefix)
{
    std::string files = "";

    for (int k = 0; k < inds.size(); ++k)
    {
        std::vector<Float> evals = std::vector<Float>();

        for (int i = 0; i < records.size(); i++)
        {
            int data_pts = records[i].getRecordByIndex(inds[k]).data_points.size();
            evals.push_back(records[i].getRecordByIndex(inds[k]).data_points[data_pts - 1].work);
        }

        std::ofstream temp_file;
        temp_file.open("../results/" + prefix + "temp_costs_" + std::to_string(inds[k]) + ".txt");
        files += "../results/" + prefix + "temp_costs_" + std::to_string(inds[k]) + ".txt " + names[k] + " ";

        for (int i = 0; i < evals.size(); ++i)
        {
            temp_file << vals[i] << std::endl;
            temp_file << evals[i] << std::endl;
        }

        temp_file.close();
    }

    std::string args = settings.compileArguements();

    std::string cmd = "python3 ../scripts/graph_funcs_tmp.py " + std::to_string(inds.size()) + " " + files + path + " " + args;
    system(cmd.c_str());
}

void Grapher::graphRecordBestBiases(std::vector<int> &inds,
                                    std::vector<std::string> &names,
                                    std::vector<ConvergenceData> &records,
                                    std::vector<double> &vals,
                                    std::string path,
                                    GraphSettings settings,
                                    std::string prefix)
{
    std::string files = "";

    for (int k = 0; k < inds.size(); ++k)
    {
        std::vector<Float> evals = std::vector<Float>();

        for (int i = 0; i < records.size(); i++)
        {
            int data_pts = records[i].getRecordByIndex(inds[k]).data_points.size();
            evals.push_back(records[i].getRecordByIndex(inds[k]).data_points[data_pts - 1].bias);
        }

        std::ofstream temp_file;
        temp_file.open("../results/" + prefix + "temp_errs_" + std::to_string(inds[k]) + ".txt");
        files += "../results/" + prefix + "temp_errs_" + std::to_string(inds[k]) + ".txt " + names[k] + " ";

        for (int i = 0; i < evals.size(); ++i)
        {
            temp_file << vals[i] << std::endl;
            temp_file << evals[i] << std::endl;
        }

        temp_file.close();
    }

    std::string args = settings.compileArguements();

    std::string cmd = "python3 ../scripts/graph_funcs_tmp.py " + std::to_string(inds.size()) + " " + files + path + " " + args;
    system(cmd.c_str());
}

void Grapher::graphRecordBestMeans(std::vector<int> &inds,
                                   std::vector<std::string> &names,
                                   std::vector<ConvergenceData> &records,
                                   std::vector<double> &vals,
                                   std::string path,
                                   GraphSettings settings,
                                   std::string prefix)
{
    std::string files = "";

    for (int k = 0; k < inds.size(); ++k)
    {
        std::vector<Float> evals = std::vector<Float>();

        for (int i = 0; i < records.size(); i++)
        {
            int data_pts = records[i].getRecordByIndex(inds[k]).data_points.size();
            evals.push_back(records[i].getRecordByIndex(inds[k]).data_points[data_pts - 1].mean);
        }

        std::ofstream temp_file;
        temp_file.open("../results/" + prefix + "temp_means_" + std::to_string(inds[k]) + ".txt");
        files += "../results/" + prefix + "temp_means_" + std::to_string(inds[k]) + ".txt " + names[k] + " ";

        for (int i = 0; i < evals.size(); ++i)
        {
            temp_file << vals[i] << std::endl;
            temp_file << evals[i] << std::endl;
        }

        temp_file.close();
    }

    std::string args = settings.compileArguements();

    std::string cmd = "python3 ../scripts/graph_funcs_tmp.py " + std::to_string(inds.size()) + " " + files + path + " " + args;
    system(cmd.c_str());
}

void Grapher::graphRecordBestMSEs(std::vector<int> &inds,
                                  std::vector<std::string> &names,
                                  std::vector<ConvergenceData> &records,
                                  std::vector<double> &vals,
                                  std::string path,
                                  GraphSettings settings,
                                  std::string prefix)
{
    std::string files = "";

    for (int k = 0; k < inds.size(); ++k)
    {
        std::vector<Float> evals = std::vector<Float>();

        for (int i = 0; i < records.size(); i++)
        {
            int data_pts = records[i].getRecordByIndex(inds[k]).data_points.size();
            evals.push_back(records[i].getRecordByIndex(inds[k]).data_points[data_pts - 1].rmse);
        }

        std::ofstream temp_file;
        temp_file.open("../results/" + prefix + "temp_mses_" + std::to_string(inds[k]) + ".txt");
        files += "../results/" + prefix + "temp_mses_" + std::to_string(inds[k]) + ".txt " + names[k] + " ";

        for (int i = 0; i < evals.size(); ++i)
        {
            temp_file << vals[i] << std::endl;
            temp_file << evals[i] << std::endl;
        }

        temp_file.close();
    }

    std::string args = settings.compileArguements();

    std::string cmd = "python3 ../scripts/graph_funcs_tmp.py " + std::to_string(inds.size()) + " " + files + path + " " + args;
    system(cmd.c_str());
}

void Grapher::graphRecordTrueAbsErrors(std::vector<int> &inds,
                                       std::vector<std::string> &names,
                                       ConvergenceData *records,
                                       std::string path,
                                       GraphSettings settings,
                                       std::string prefix)
{
    std::string files = "";

    for (int k = 0; k < inds.size(); ++k)
    {
        ConvergenceRecord record = records->getRecordByIndex(inds[k]);

        std::vector<Float> xs = std::vector<Float>();
        std::vector<Float> evals = std::vector<Float>();

        for (int i = 0; i < record.data_points.size(); i++)
        {
            xs.push_back(record.data_points[i].samples);
            evals.push_back(record.data_points[i].true_abs_diff);
        }

        std::ofstream temp_file;
        temp_file.open("../results/" + prefix + "temp_true_errs_" + std::to_string(inds[k]) + ".txt");

        files += "../results/" + prefix + "temp_true_errs_" + std::to_string(inds[k]) + ".txt " + names[k] + " ";

        for (int i = 0; i < evals.size(); ++i)
        {
            temp_file << xs[i] << std::endl;
            temp_file << evals[i] << std::endl;
        }

        temp_file.close();
    }

    std::string args = settings.compileArguements();
    std::cout << "ARGS: " << args << std::endl;

    std::string cmd = "python3 ../scripts/graph_funcs_tmp.py " + std::to_string(inds.size()) + " " + files + path + " " + args;
    system(cmd.c_str());
}

void Grapher::graphRecordSampAbsErrors(std::vector<int> &inds,
                                       std::vector<std::string> &names,
                                       ConvergenceData *records,
                                       std::string path,
                                       GraphSettings settings,
                                       std::string prefix)
{
    std::string files = "";

    for (int k = 0; k < inds.size(); ++k)
    {
        ConvergenceRecord record = records->getRecordByIndex(inds[k]);

        std::vector<Float> xs = std::vector<Float>();
        std::vector<Float> evals = std::vector<Float>();

        for (int i = 0; i < record.data_points.size(); i++)
        {
            xs.push_back(record.data_points[i].samples);
            evals.push_back(record.data_points[i].sample_abs_diff);
        }

        std::ofstream temp_file;
        temp_file.open("../results/" + prefix + "temp_sample_errs_" + std::to_string(inds[k]) + ".txt");

        files += "../results/" + prefix + "temp_sample_errs_" + std::to_string(inds[k]) + ".txt " + names[k] + " ";

        for (int i = 0; i < evals.size(); ++i)
        {
            temp_file << xs[i] << std::endl;
            temp_file << evals[i] << std::endl;
        }

        temp_file.close();
    }

    std::string args = settings.compileArguements();
    std::cout << "ARGS: " << args << std::endl;

    std::string cmd = "python3 ../scripts/graph_funcs_tmp.py " + std::to_string(inds.size()) + " " + files + path + " " + args;
    system(cmd.c_str());
}

void Grapher::graphRecordSampMSEs(std::vector<int> &inds,
                                  std::vector<std::string> &names,
                                  ConvergenceData *records,
                                  std::string path,
                                  GraphSettings settings,
                                  std::string prefix)
{
    std::string files = "";

    for (int k = 0; k < inds.size(); ++k)
    {
        ConvergenceRecord record = records->getRecordByIndex(inds[k]);

        std::vector<Float> xs = std::vector<Float>();
        std::vector<Float> evals = std::vector<Float>();

        for (int i = 0; i < record.data_points.size(); i++)
        {
            xs.push_back(record.data_points[i].samples);
            evals.push_back(record.data_points[i].sample_mse);
        }

        std::ofstream temp_file;
        temp_file.open("../results/" + prefix + "temp_samp_mses_" + std::to_string(inds[k]) + ".txt");

        files += "../results/" + prefix + "temp_samp_mses_" + std::to_string(inds[k]) + ".txt " + names[k] + " ";

        for (int i = 0; i < evals.size(); ++i)
        {
            temp_file << xs[i] << std::endl;
            temp_file << evals[i] << std::endl;
        }

        temp_file.close();
    }

    std::string args = settings.compileArguements();
    std::cout << "ARGS: " << args << std::endl;

    std::string cmd = "python3 ../scripts/graph_funcs_tmp.py " + std::to_string(inds.size()) + " " + files + path + " " + args;
    system(cmd.c_str());
}

void Grapher::graphRecordTrueMSEs(std::vector<int> &inds,
                                  std::vector<std::string> &names,
                                  ConvergenceData *records,
                                  std::string path,
                                  GraphSettings settings,
                                  std::string prefix)
{
    std::string files = "";

    for (int k = 0; k < inds.size(); ++k)
    {
        ConvergenceRecord record = records->getRecordByIndex(inds[k]);

        std::vector<Float> xs = std::vector<Float>();
        std::vector<Float> evals = std::vector<Float>();

        for (int i = 0; i < record.data_points.size(); i++)
        {
            xs.push_back(record.data_points[i].samples);
            evals.push_back(record.data_points[i].true_mse);
        }

        std::ofstream temp_file;
        temp_file.open("../results/" + prefix + "temp_true_mses_" + std::to_string(inds[k]) + ".txt");

        files += "../results/" + prefix + "temp_true_mses_" + std::to_string(inds[k]) + ".txt " + names[k] + " ";

        for (int i = 0; i < evals.size(); ++i)
        {
            temp_file << xs[i] << std::endl;
            temp_file << evals[i] << std::endl;
        }

        temp_file.close();
    }

    std::string args = settings.compileArguements();
    std::cout << "ARGS: " << args << std::endl;

    std::string cmd = "python3 ../scripts/graph_funcs_tmp.py " + std::to_string(inds.size()) + " " + files + path + " " + args;
    system(cmd.c_str());
}
