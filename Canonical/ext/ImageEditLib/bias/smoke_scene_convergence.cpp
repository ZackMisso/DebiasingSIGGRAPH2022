#include <iostream>
#include <fstream>
#include <tgmath.h>
#include <pcg32.h>
#include "imedit/image.h"
#include "imedit/im_util.h"

// TODO: fully convert this script
int main(int argc, char* argv[])
{
    imedit::Image img_one = imedit::Image("/Users/corneria/Documents/Research/bias_research/pbrttest/results/smoke_5/512_unb_step/volpath.exr");
    imedit::Image img_two = imedit::Image("/Users/corneria/Documents/Research/bias_research/pbrttest/results/smoke_5/512_unb_step/volpath_ps_16384.exr");
    imedit::Image img_three = imedit::Image("/Users/corneria/Documents/Research/bias_research/pbrttest/results/smoke_5/512_unb_step/volpath_ps_8192.exr");
    imedit::Image gt = imedit::Image("/Users/corneria/Documents/Research/bias_research/pbrttest/results/smoke_5/gt.exr");

    std::cout << "MSE one: " << imedit::mean_sqr_error(gt, img_one) << std::endl;
    std::cout << "MSE two: " << imedit::mean_sqr_error(gt, img_two) << std::endl;
    std::cout << "MSE three: " << imedit::mean_sqr_error(gt, img_three) << std::endl;

    return 0;
}
