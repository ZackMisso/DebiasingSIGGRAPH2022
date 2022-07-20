#include <iostream>
#include <tgmath.h>
#include "imedit/image.h"
#include "imedit/im_util.h"
#include "imedit/procedural.h"
#include "imedit/im_color_maps.h"

#define PI 3.1415269

float image_eval(float x, float y)
{

}

int main(int argc, char* argv[])
{
    std::string publish_command = "ffmpeg -r 60 -f image2 -i " "wavy/frame_%03d.png -vcodec mpeg4 -vb 20M -minrate 20M -maxrate 30M wavy/wavy.mp4";
    imedit::Image smiley = imedit::Image("../data/smile_2.png");
    imedit::thresh_min_image(smiley, 0.3);

    int groups = 0;
    imedit::Image grouped_image = imedit::group_white(smiley, groups);

    for (int k = 0; k < 1; ++k)
    {
        std::cout << "rendering frame: " << k << std::endl;
        imedit::Image image = imedit::Image(1024, 1024);

        float amp = float(k) / 1000.0 * 0.031;

        for (int i = 0; i < image.height(); ++i)
        {
            for (int j = 0; j < image.width(); ++j)
            {
                float val = image_eval(j * cos(std::abs(256 - i) * amp), 0.0);
                // float val = image_eval(j);
                image(j, i, 0) = val;
                image(j, i, 1) = val;
                image(j, i, 2) = val;
            }
        }

        char str[4];
        snprintf(str, 4, "%03d", k);

        smiley.write("smiley/frame_" + std::string(str) + ".png");
        grouped_image.write("smiley/grouped_frame_" + std::string(str) + ".png");
    }

    // system(publish_command.c_str());

    return 0;
}
