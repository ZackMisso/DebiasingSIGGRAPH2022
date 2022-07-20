/*
    ImageEditLib was developed by Zack Misso
    <zackary.t.misso.gr@dartmouth.edu>

    All code here is free to use as long as
    proper credit is given to the author
*/

#include <iostream>
#include "imedit/image.h"
#include "imedit/procedural.h"
#include "imedit/im_util.h"

int main()
{
    imedit::Image noise_image = imedit::Image(1024, 1024, 3);

    imedit::noise_image_xy(noise_image,
                           80.0,
                           0.0,
                           0.0,
                           0.0);

    // noise_image = (noise_image + 1.0) * 0.5;

    noise_image.write("sandbox/noise.exr");

    imedit::Image turb_image = imedit::Image(1024, 1024, 3);

    imedit::turbulence_image_xy(turb_image,
                                36.0,
                                0.0,
                                0.0,
                                0.0);

    turb_image.write("sandbox/turb.exr");

    float max = turb_image.max();

    // turb_image = turb_image - max / 2.0;

    noise_image += turb_image;

    noise_image.write("sandbox/final_noise.png");

    return 0;
}
