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
    int start_frame = 0;
    int end_frame = 4920;

    std::string prefix = "/Users/corneria/Documents/Projects/FeignRenderer/build/hall_of_tiles_final/hall_of_tiles_final_";

    for (int i = start_frame; i < end_frame; ++i)
    {
        char str[5];
        snprintf(str, 5, "%04d", i);
        std::string frame = std::string(str);
        imedit::Image image;
        bool success = image.read(prefix + frame + ".exr");
        if (!success) std::cout << frame << std::endl;
    }
    
    return 0;
}
