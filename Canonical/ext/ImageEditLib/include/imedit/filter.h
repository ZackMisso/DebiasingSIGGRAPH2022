#pragma once

#include "image.h"
#include <vector>

namespace imedit
{

void sharpen3x3(const Image& old, Image& image)
{
    // filter is of the form:
    // 0  -1  0
    // -1  4 -1
    // 0  -1  0

    for (int k = 0; k < image.depth(); ++k)
    {
        for (int i = 1; i < image.height() - 1; ++i)
        {
            for (int j = 1; j < image.width() - 1; ++j)
            {
                Float up_val = old.filter_index(j, i-1, k);
                Float down_val = old.filter_index(j, i+1, k);
                Float right_val = old.filter_index(j-1, i, k);
                Float left_val = old.filter_index(j+1, i, k);
                Float val = old.filter_index(j, i, k);

                Float new_val = -1.0 * (up_val + down_val + right_val + left_val) +
                             4.0 * val;

                image(j, i, k) = new_val;
            }
        }
    }
}

}
