#include <iostream>
#include <imedit/image.h>
#include <fstream>

void get_bounding_box(const imedit::Image& mask,
                      int& min_x,
                      int& min_y,
                      int& max_x,
                      int& max_y)
{
    bool found_first = false;
    for (int i = 0; i < mask.height(); i++)
    {
        for (int j = 0; j < mask.width(); ++j)
        {
            if (mask(j, i).r > 1e-4)
            {
                if (!found_first)
                {
                    min_x = j;
                    min_y = i;
                    max_x = j;
                    max_y = i;
                    found_first = true;
                }
                else
                {
                    min_x = std::min(min_x, j);
                    min_y = std::min(min_y, i);
                    max_x = std::max(max_x, j);
                    max_y = std::max(max_y, i);
                }
            }
        }
    }
}

int main(int argc, char* argv[])
{
    std::string path = std::string(argv[1]);
    std::string name = std::string(argv[2]);

    imedit::Image img = imedit::Image(path + "img_" + name + ".exr");
    imedit::Image mask = imedit::Image(path + "mask_" + name + ".exr");

    int min_x, min_y, max_x, max_y;
    get_bounding_box(mask, min_x, min_y, max_x, max_y);

    std::ofstream file;
    file.open(path + "bbox_" + name + ".txt");
    file << min_x << std::endl;
    file << min_y << std::endl;
    file << max_x << std::endl;
    file << max_y << std::endl;
    file.close();

    img.write(path + "img_" + name + ".png");
    mask.write(path + "mask_" + name + ".png");
}
