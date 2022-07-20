#include <iostream>
#include <tgmath.h>
#include "imedit/image.h"
#include "imedit/im_util.h"

void write_pix(imedit::Pixel pix)
{
    std::cout << "(" << pix.r << ", " << pix.g << ", " << pix.b << ")";
}

int main(int argc, char* argv[])
{
    imedit::Image image = imedit::Image(1024, 1024);

    for (int i = 0; i < 1024; ++i)
    {
        for (int j = 0; j < 1024; ++j)
        {
            float sat = 1.0 - float(i) / 1024.0;
            float hue = float(j) / 1024.0;

            imedit::Pixel hsl = imedit::Pixel(hue, sat, 0.5);
            if (i == 0)
                write_pix(hsl);
            imedit::hsl_to_rgb(hsl);
            if (i == 0)
            {
                std::cout << " -> ";
                write_pix(hsl);
                std::cout << std::endl;
            }

            image(j, i) = hsl;
        }
    }

    imedit::Pixel test_1 = imedit::Pixel(59.0 / 360.0, 1.0, 0.5);
    imedit::Pixel test_2 = imedit::Pixel(61.0 / 360.0, 1.0, 0.5);

    imedit::hsl_to_rgb(test_1);
    imedit::hsl_to_rgb(test_2);

    std::cout << "test:" << std::endl;
    write_pix(test_1);
    std::cout << std::endl;
    write_pix(test_2);

    image.write("hsl_mapping.exr");

    return 0;
}
