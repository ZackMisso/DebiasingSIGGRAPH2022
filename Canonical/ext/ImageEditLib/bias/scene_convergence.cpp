#include <iostream>
#include <fstream>
#include <tgmath.h>
#include <pcg32.h>
#include "imedit/image.h"
#include "imedit/im_util.h"

// TODO: fully convert this script
int main(int argc, char* argv[])
{
    if (argc != 5)
    {
        std::cout << "ERROR: this program requires 5 inputs" << std::endl;
        return 0;
    }

    std::string prefix_path = std::string(argv[1]);
    std::string output_path = std::string(argv[2]);
    std::string gt_path = std::string(argv[3]);
    int pows = std::stoi(argv[4]);

    imedit::Image gt = imedit::Image(gt_path);

    std::ofstream file;
    file.open(output_path + "mse.txt");

    for (int i = 0; i < pows; ++i)
    {
        int ps = std::pow(2, i);

        imedit::Image image = imedit::Image(prefix_path + std::to_string(ps) + ".exr");

        file << imedit::mean_sqr_error(gt, image) << std::endl;
    }

    file.close();

    return 0;
}
