#include <iostream>
#include <fstream>
#include <tgmath.h>
#include <pcg32.h>
#include "imedit/image.h"
#include "imedit/im_util.h"
#include "imedit/procedural.h"
#include "imedit/im_color_maps.h"

int main(int argc, char* argv[])
{
    std::ofstream image_code;
    image_code.open("image_code.txt");

    for (int k = 0; k < 8; ++k)
    {
        std::string image_path = "/Users/corneria/Documents/Art/sonic_run/run_" + std::to_string(k) + ".png";

        imedit::Image image = imedit::Image(image_path);

        image_code << "vec3 image_" << k << "(vec2 uv, float time)" << std::endl;
        image_code << "{" << std::endl;
        image_code << "    uv.x -= 0.25;" << std::endl;
        image_code << "    uv.x *= aspect;" << std::endl;
        image_code << "    ivec2 pixel_index = ivec2(int(floor(uv.x * 50.f * 1.f)), int(floor(50.f - uv.y * 50.f)));" << std::endl;
        image_code << "    vec3 color = vec3(0.0);" << std::endl;

        for (int i = 0; i < image.height(); i++)
        {
            for (int j = 0; j < image.height(); j++)
            {
                imedit::Pixel pix = image(j, i);

                if (pix != imedit::Pixel(1.f))
                {
                    image_code << "    color += chk(pixel_index, ivec2(" << j << ", " << i << ")) * (vec3(" << pix.r << ", " << pix.g << ", " << pix.b << "));" << std::endl;
                }
            }
        }

        image_code << "    return color;" << std::endl;
        image_code << "}" << std::endl;
    }

    return 0;
}
