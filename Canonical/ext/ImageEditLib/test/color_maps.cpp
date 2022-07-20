#include <iostream>
#include "imedit/image.h"
#include "imedit/procedural.h"
#include "imedit/im_color_maps.h"

int main(int argc, char* argv[])
{
    imedit::Image image = imedit::Image(1024, 1024, 3);

    std::vector<imedit::Pixel> colors = std::vector<imedit::Pixel>();
    imedit::color_map_viridis(colors);

    for (int i = 0; i < image.height(); ++i)
    {
        for (int j = 0; j < image.width(); ++j)
        {
            float proxy = float(j) / float(image.width() - 1);
            imedit::Pixel color = imedit::color_from_proxy(colors, proxy);

            image(j, i, 0) = color.r;
            image(j, i, 1) = color.g;
            image(j, i, 2) = color.b;
        }
    }

    image.write("viridis_map.png");

    return 0;
}
