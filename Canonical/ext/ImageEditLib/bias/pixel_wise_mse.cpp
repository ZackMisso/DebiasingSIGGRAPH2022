#include <iostream>
#include <fstream>
#include <tgmath.h>
#include <pcg32.h>
#include "imedit/image.h"
#include "imedit/im_util.h"

double avg(double x,
           double y,
           double z)
{
    return (x + y + z) / 3.0;
}

double norm(double x,
            double y,
            double z)
{
    return std::sqrt(x*x + y*y + z*z);
}

double sqr_norm(double x,
                double y,
                double z)
{
    return x*x + y*y + z*z;
}

double max(double x,
           double y,
           double z)
{
    return std::max(x, std::max(y, z));
}

double min(double x,
           double y,
           double z)
{
    return std::min(x, std::min(y, z));
}

int main(int argc, char* argv[])
{
    if (argc != 5)
    {
        std::cout << "ERROR: this program requires 5 inputs" << std::endl;
        return 0;
    }

    std::string im_directory = std::string(argv[1]);
    std::string gt_image_name = std::string(argv[2]);
    int start = std::stoi(argv[3]);
    int end = std::stoi(argv[4]); // end n, n+1 still has to exist

    imedit::Image gt_image = imedit::Image(gt_image_name);

    int width = gt_image.width();
    int height = gt_image.height();

    // create the difference images
    // for (int i = start; i < end; ++i)
    // {
    //     imedit::Image inp1 = imedit::Image(im_directory + std::to_string(i+1) + ".exr");
    //     imedit::Image in = imedit::Image(im_directory + std::to_string(i) + ".exr");
    //
    //     imedit::Image diff = inp1-in;
    //     imedit::im_abs(diff);
    //
    //     diff.write(im_directory + std::to_string(i) + "_" + std::to_string(i+1) + ".exr");
    // }

    // create actual data
    std::ofstream file;
    file.open(im_directory + "pw_mse.txt");
    file << width << std::endl;
    file << height << std::endl;
    file << start << std::endl;
    file << end << std::endl;

    std::vector<imedit::Image> images = std::vector<imedit::Image>();
    std::vector<imedit::Image> var_images = std::vector<imedit::Image>();

    for (int k = start; k <= end; ++k)
    {
        images.push_back(imedit::Image(im_directory + std::to_string(k) + ".exr"));
    }

    for (int k = start; k < end; ++k)
    {
        std::cout << "PARSING: " << im_directory + "var_" +
                                           std::to_string(k) + "_" +
                                           std::to_string(k+1) + ".exr" << std::endl;
        var_images.push_back(imedit::Image(im_directory + "var_" +
                                           std::to_string(k) + "_" +
                                           std::to_string(k+1) + ".txt"));
    }

    for (int i = 0; i < height; ++i)
    {
        for (int j = 0; j < width; ++j)
        {
            std::vector<double> diffs = std::vector<double>();
            double sum = 0.0;

            for (int k = 0; k < end - start; ++k)
            {
                // take the norm of the differences per pixel channel
                double diff_x = std::abs(images[k+1](j, i, 0) - images[k](j, i, 0));
                double diff_y = std::abs(images[k+1](j, i, 1) - images[k](j, i, 1));
                double diff_z = std::abs(images[k+1](j, i, 2) - images[k](j, i, 2));

                double expected_qr = max(diff_x*diff_x,
                                         diff_y*diff_y,
                                         diff_z*diff_z);

                double var_x = var_images[k](j, i, 0);
                double var_y = var_images[k](j, i, 0);
                double var_z = var_images[k](j, i, 0);

                double var = max(var_x, var_y, var_z);

                if (isinf(var) || isnan(var))
                {
                    var = 0.0;
                }

                double pmf_i = std::sqrt(var + expected_qr);

                // var + I^2
                diffs.push_back(pmf_i);

                // if (i == 0 && j == 0)
                // {
                //     std::cout << "k: " << k << std::endl;
                //     std::cout << "k: " << k << " EXPECTED: " << expected_qr << std::endl;
                //     std::cout << "k: " << k << " Var: " << var << std::endl;
                //     std::cout << "var_x: " << var_x << std::endl;
                //     std::cout << "var_y: " << var_y << std::endl;
                //     std::cout << "var_z: " << var_z << std::endl;
                //
                //     // var_images[k].write("wtf_man.exr");
                //     // var_images[k].write("wtf_man.png");
                // }

                sum += pmf_i;
            }

            for (int k = 0; k < diffs.size(); ++k)
            {
                file << diffs[k] / sum << " ";
            }

            file << std::endl;
        }
    }

    file.close();

    return 0;
}
