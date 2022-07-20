#include <conver/conv_data.h>

ConvergenceRecord::ConvergenceRecord()
{
    data_points = std::vector<DataPoint>();
    tech_name = "";
}

ConvergenceRecord::ConvergenceRecord(std::string tech_name) : tech_name(tech_name)
{
    data_points = std::vector<DataPoint>();
}

ConvergenceRecord::ConvergenceRecord(const ConvergenceRecord &other)
    : tech_name(other.tech_name)
{
    data_points = std::vector<DataPoint>();

    for (int i = 0; i < other.data_points.size(); ++i)
    {
        data_points.push_back(other.data_points[i]);
    }
}

void ConvergenceRecord::add_datapoint(DataPoint &point)
{
    data_points.push_back(point);
}

void ConvergenceRecord::add(const ConvergenceRecord &other)
{
    // TODO: This should be removed in favor of robust evaluation
    for (int i = 0; i < data_points.size(); ++i)
    {
        data_points[i].variance += other.data_points[i].variance;
        data_points[i].work += other.data_points[i].work;
        data_points[i].work_norm_var += other.data_points[i].work_norm_var;
        data_points[i].mean += other.data_points[i].mean;
        data_points[i].mse += other.data_points[i].mse;
        data_points[i].rmse += other.data_points[i].rmse;
        data_points[i].bias += other.data_points[i].bias;
    }
}

void ConvergenceRecord::normalize(int n)
{
    // TODO: This should be removed in favor of robust evaluation
    for (int i = 0; i < data_points.size(); ++i)
    {
        data_points[i].variance /= Float(n);
        data_points[i].work /= Float(n);
        data_points[i].work_norm_var /= Float(n);
        data_points[i].mean /= Float(n);
        data_points[i].mse /= Float(n);
        data_points[i].rmse /= Float(n);
        data_points[i].bias /= Float(n);
    }
}

ConvergenceData::ConvergenceData()
{
    data_records = std::vector<ConvergenceRecord>();
}

ConvergenceData::ConvergenceData(int num_techniques)
{
    data_records = std::vector<ConvergenceRecord>(num_techniques);
}

ConvergenceData::ConvergenceData(const std::vector<ConvergenceRecord> &records)
{
    data_records = records;
}

void ConvergenceData::saveDataToFile(std::string path) const
{
    throw new NotImplementedException("conv data save to file");
}

void ConvergenceData::readDataFromFile(std::string path)
{
    throw new NotImplementedException("conv data read from file");
}

void ConvergenceData::addTechnique(std::string tech_name)
{
    data_records.push_back(ConvergenceRecord(tech_name));
}

void ConvergenceData::addRecord(ConvergenceRecord &record)
{
    data_records.push_back(record);
}

void ConvergenceData::setTechniqueNameByIndex(std::string name, int index)
{
    data_records[index].tech_name = name;
}

ConvergenceRecord &ConvergenceData::getRecordByIndex(int index)
{
    return data_records[index];
}

bool ConvergenceData::getRecordByTechnique(std::string name, ConvergenceRecord &record)
{
    for (int i = 0; i < data_records.size(); ++i)
    {
        if (data_records[i].tech_name == name)
        {
            record = data_records[i];
            return true;
        }
    }

    return false;
}