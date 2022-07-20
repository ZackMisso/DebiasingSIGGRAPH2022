#include <iostream>
#include <fstream>
#include <tgmath.h>
#include <pcg32.h>
#include "imedit/image.h"
#include "imedit/im_util.h"

int main(int argc, char* argv[])
{
    if (argc != 4)
    {
        std::cout << "ERROR: this program requires 4 inputs" << std::endl;
        return 0;
    }

    std::string gt_image_loc = std::string(argv[1]);
    std::string im_directory = std::string(argv[2]);
    int total_pix_samps = std::stoi(argv[3]);

    imedit::Image gt_image = imedit::Image(gt_image_loc);
    std::vector<double> mses = std::vector<double>();
    std::vector<int> samps = std::vector<int>();

    if (total_pix_samps == 0)
    {
        imedit::Image image = imedit::Image(im_directory + ".exr");
        double mse = imedit::mean_sqr_error(gt_image, image);

        std::ofstream file;
        file.open(im_directory + "_mse.txt");
        file << 1 << std::endl;
        file << mse << std::endl;

        file.close();
    }
    else
    {
        for (int i = 1; i <= total_pix_samps; i <<= 1)
        {
            imedit::Image image = imedit::Image(im_directory + "_ps_" +
                                                std::string(std::to_string(i)) + ".exr");

            mses.push_back(imedit::mean_sqr_error(gt_image, image));
            samps.push_back(i);
        }

        std::ofstream file;
        file.open(im_directory + "_mse.txt");
        for (int i = 0; i < mses.size(); ++i)
        {
            file << samps[i] << std::endl;
            file << mses[i] << std::endl;
        }

        file.close();
    }

    return 0;
}
