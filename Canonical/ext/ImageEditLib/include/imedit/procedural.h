/*
    ImageEditLib was developed by Zack Misso
    <zackary.t.misso.gr@dartmouth.edu>

    All code here is free to use as long as
    proper credit is given to the author
*/

#include "image.h"
#include "noise.h"
#include <tgmath.h>

namespace imedit
{

// including z axis for now in case I want to extend this to support procedural
// voxel maps in the future
enum ImagePartition
{
    IP_X, // x axis only
    IP_Y, // y axis only
    // IP_Z, // z axis only

    // Should these be supported???
    // IP_R, // r channel only
    // IP_G, // g channel only
    // IP_B, // b channel only
    // IP_A, // a channel only

    IP_XY, // x and y axis
    // IP_XZ, // x and z axis
    // IP_YZ, // y and z axis
    // IP_XYZ, // all axis

    // should these be supported ???
    // IP_RG, // r and g channels only
    // IP_RB, // r and b channels only
    // IP_RA, // r and a channels only
    // IP_GB, // g and b channels only
    // IP_GA, // g and a channels only
    // IP_BA, // b and a channels only
    //
    // IP_RGB, // r g, and b channels only
    // IP_RGA, // r g, and a channels only
    // IP_RBA, // r b, and a channels only
    // IP_GBA, // g b, and a channels only
    // IP_RGBA // all channels
};

// void marble_image()

// TODO: maybe connect this to Vec implementation to use instead of pairs

static void euclidean_dist_from_points_image(Image& image,
                                             std::vector<std::pair<Float,Float> > pairs)
{
    for (int i = 0; i < image.height(); ++i)
    {
        for (int j = 0; j < image.width(); ++j)
        {
            Float val = 10000000.0;

            for (int k = 0; k < pairs.size(); ++k)
            {
                Float j_val = pairs[k].first - (Float(j) + Float(0.5));
                Float i_val = pairs[k].second - (Float(i) + Float(0.5));

                j_val *= j_val;
                i_val *= i_val;

                Float tmp = sqrt(j_val + i_val);

                if (tmp < val) val = tmp;
            }

            for (int k = 0; k < image.depth(); ++k)
            {
                image(j, i, k) = val;
            }
        }
    }
}

static void manhattan_dist_from_points_image(Image& image,
                                             std::vector<std::pair<int,int> > pairs)
{
    for (int i = 0; i < image.height(); ++i)
    {
        for (int j = 0; j < image.width(); ++j)
        {
            int val = 10000000;

            for (int k = 0; k < pairs.size(); ++k)
            {
                int j_val = std::abs(pairs[k].first - j);
                int i_val = std::abs(pairs[k].second - i);

                int tmp = j_val + i_val;

                if (tmp < val) val = tmp;
            }

            for (int k = 0; k < image.depth(); ++k)
            {
                image(j, i, k) = val;
            }
        }
    }
}

static void euclidean_tiled_image(Image& image,
                                  const std::vector<std::pair<Float, Float> >& pairs,
                                  const std::vector<Pixel>& pixels)
{
    for (int i = 0; i < image.height(); ++i)
    {
        for (int j = 0; j < image.width(); ++j)
        {
            int best_index = -1;
            Float val = image.width() * image.height();

            for (int k = 0; k < pairs.size(); ++k)
            {
                Float j_val = pairs[k].first - (Float(j) + Float(0.5));
                Float i_val = pairs[k].second - (Float(i) + Float(0.5));

                j_val *= j_val;
                i_val *= i_val;

                Float tmp = sqrt(j_val + i_val);

                if (tmp < val)
                {
                    val = tmp;
                    best_index = k;
                }
            }

            image.setPixel(j, i, pixels[best_index]);
        }
    }
}

static void euclidean_edge_image(Image& image,
                                 const std::vector<std::pair<Float, Float> >& pairs,
                                 Float threshold = -1.0)
{
    if (threshold < 0.0)
        threshold = 3.0;

    for (int i = 0; i < image.height(); ++i)
    {
        for (int j = 0; j < image.width(); ++j)
        {
            int best_index = -1;
            int second_best_index = -1;
            Float best_val = image.width() * image.height();
            Float second_best_val = image.width() * image.height();

            for (int k = 0; k < pairs.size(); ++k)
            {
                Float j_val = pairs[k].first - (Float(j) + Float(0.5));
                Float i_val = pairs[k].second - (Float(i) + Float(0.5));

                j_val *= j_val;
                i_val *= i_val;

                Float tmp = sqrt(j_val + i_val);

                if (tmp < best_val)
                {
                    second_best_val = best_val;
                    best_val = tmp;
                    second_best_index = best_index;
                    best_index = k;
                }
                else if (tmp < second_best_val)
                {
                    second_best_val = tmp;
                    second_best_index = k;
                }
            }

            if (abs(best_val - second_best_val) < threshold)
            {
                image(j, i, 0) = 1.0;
                image(j, i, 1) = 1.0;
                image(j, i, 2) = 1.0;
            }
        }
    }
}

static void manhattan_tiled_image(Image& image,
                                  const std::vector<std::pair<int,int> >& pairs,
                                  const std::vector<Pixel>& pixels)
{
    for (int i = 0; i < image.height(); ++i)
    {
        for (int j = 0; j < image.width(); ++j)
        {
            int val = image.width() * image.height();
            int best_index = -1;

            for (int k = 0; k < pairs.size(); ++k)
            {
                int j_val = std::abs(pairs[k].first - j);
                int i_val = std::abs(pairs[k].second - i);

                int tmp = j_val + i_val;

                if (tmp < val)
                {
                    val = tmp;
                    best_index = k;
                }
            }

            image.setPixel(j, i, pixels[best_index]);
        }
    }
}

static void manhattan_tiled_image(Image& image,
                                  const std::vector<std::pair<Float,Float> >& pairs,
                                  const std::vector<Pixel>& pixels)
{
    std::vector<std::pair<int,int> > new_pairs = std::vector<std::pair<int,int> >(pairs.size());

    for (int i = 0; i < pairs.size(); ++i)
    {
        new_pairs[i].first = (int)pairs[i].first;
        new_pairs[i].second = (int)pairs[i].second;
    }

    manhattan_tiled_image(image,
                          new_pairs,
                          pixels);
}

static void euclidean_dist_image(Image& image, Float xpos, Float ypos)
{
    for (int i = 0; i < image.height(); ++i)
    {
        for (int j = 0; j < image.width(); ++j)
        {
            Float j_val = xpos - (Float(j) + Float(0.5));
            Float i_val = ypos - (Float(i) + Float(0.5));

            j_val *= j_val;
            i_val *= i_val;

            Float val = sqrt(j_val + i_val);

            for (int k = 0; k < image.depth(); ++k)
            {
                image(j, i, k) = val;
            }
        }
    }
}

static void euclidean_dist_image(Image& image,
                                 const std::vector<std::pair<Float, Float> >& pairs)
{
    for (int i = 0; i < image.height(); ++i)
    {
        for (int j = 0; j < image.width(); ++j)
        {
            int best_index = -1;
            Float val = image.width() * image.height();

            for (int k = 0; k < pairs.size(); ++k)
            {
                Float j_val = pairs[k].first - (Float(j) + Float(0.5));
                Float i_val = pairs[k].second - (Float(i) + Float(0.5));

                j_val *= j_val;
                i_val *= i_val;

                Float tmp = sqrt(j_val + i_val);

                if (tmp < val)
                {
                    val = tmp;
                    best_index = k;
                }
            }

            image(j, i, 0) = val;
            image(j, i, 1) = val;
            image(j, i, 2) = val;
        }
    }
}

static void manhattan_dist_image(Image& image, int xpos, int ypos)
{
    for (int i = 0; i < image.height(); ++i)
    {
        for (int j = 0; j < image.width(); ++j)
        {
            int j_val = std::abs(xpos - j);
            int i_val = std::abs(ypos - i);

            for (int k = 0; k < image.depth(); ++k)
            {
                image(j, i, k) = j_val + i_val;
            }
        }
    }
}

// Note: I know this is inefficient - TODO: fix
static void splat_manhattan(Image& image,
                            Pixel color,
                            int xpos,
                            int ypos,
                            int radius)
{
    for (int i = 0; i < image.height(); ++i)
    {
        for (int j = 0; j < image.width(); ++j)
        {
            int j_val = std::abs(xpos - j);
            int i_val = std::abs(ypos - i);

            if (j_val + i_val < radius)
            {
                image(j, i, 0) += color.r;
                image(j, i, 1) += color.g;
                image(j, i, 2) += color.b;
            }
        }
    }
}

// Note: I know this is inefficient - TODO: fix
static void splat_euclidean(Image& image,
                            Pixel color,
                            Float xpos,
                            Float ypos,
                            Float radius)
{
    for (int i = 0; i < image.height(); ++i)
    {
        for (int j = 0; j < image.width(); ++j)
        {
            Float j_val = xpos - (Float(j) + Float(0.5));
            Float i_val = ypos - (Float(i) + Float(0.5));

            j_val *= j_val;
            i_val *= i_val;

            Float val = sqrt(j_val + i_val);

            if (val < radius)
            {
                image(j, i, 0) += color.r;
                image(j, i, 1) += color.g;
                image(j, i, 2) += color.b;
            }
        }
    }
}

// template <typename T>
// void turbulence_image_xyz(Image<T>& image,
//                           T period,
//                           T xtrans = T(0.0))
//                           //T ytrans = T(0.0),
//                           //T ztrans = T(0.0))
// {
// //     for (int i = 0; i < image.height(); ++i)
// //     {
// //         for (int j = 0; j < image.width(); ++j)
// //         {
// //             for (int k = 0; k < image.depth(); ++k)
// //             {
// //                 image(j, i, k) = turbulence(T(j + 0.5) + xtrans,
// //                                             T(i + 0.5) + ytrans,
// //                                             T(k + 0.5) + ztrans,
// //                                             period);
// //             }
// //         }
// //     }
// }

static void turbulence_image_xy(Image& image,
                                Float period,
                                Float xtrans = (Float)0.0,
                                Float ytrans = (Float)0.0,
                                Float zloc = (Float)0.0)
{
    for (int i = 0; i < image.height(); ++i)
    {
        for (int j = 0; j < image.width(); ++j)
        {
            Float val = turbulence(Float(j + 0.5) + xtrans,
                                   Float(i + 0.5) + ytrans,
                                   zloc,
                                   period);

            for (int k = 0; k < image.depth(); ++k)
            {
                image(j, i, k) = val;
            }
        }
    }
}

static void turbulence_image_xyz(Image& image,
                                 Float period,
                                 Float xtrans = (Float)0.0,
                                 Float ytrans = (Float)0.0,
                                 Float ztrans = (Float)0.0)
{
    if (image.depth() == 3)
    {
        for (int i = 0; i < image.height(); ++i)
        {
            for (int j = 0; j < image.width(); ++j)
            {
                Float val = turbulence(j + 0.5f + xtrans,
                                       i + 0.5f + ytrans,
                                       0.5f + ztrans,
                                       period);

                for (int k = 0; k < image.depth(); ++k)
                {
                    image(j, i, k) = val;
                }
            }
        }
    }
    else if (image.depth() % 3 == 0)
    {
        // TODO
    }
    else
    {
        // not supported
    }
}

static void noise_image(Image& image,
                        Float period,
                        Float xtrans = 0.0,
                        Float ytrans = 0.0,
                        Float ztrans = 0.0)
{
    if (image.depth() == 3)
    {
        for (int i = 0; i < image.height(); ++i)
        {
            for (int j = 0; j < image.width(); ++j)
            {
                for (int k = 0; k < image.depth(); ++k)
                {
                    image(j, i, k) = noise(j + 0.5f + xtrans,
                                           i + 0.5f + ytrans,
                                           k + 0.5f + ztrans,
                                           period);
                }
            }
        }
    }
}

static void noise_image_xy(Image& image,
                           Float period,
                           Float xtrans = 0.0,
                           Float ytrans = 0.0,
                           Float zloc = 0.0)
{
    for (int i = 0; i < image.height(); ++i)
    {
        for (int j = 0; j < image.width(); ++j)
        {
            Float val = noise(Float(j + 0.5) + xtrans,
                              Float(i + 0.5) + ytrans,
                              zloc,
                              period);

            for (int k = 0; k < image.depth(); ++k)
            {
                image(j, i, k) = val;
            }
        }
    }
}

static Image mirror_along_x_axis(const Image& image, int location)
{
    Image ret = Image(image.width(), image.height());

    for (int i = 0; i < image.height(); ++i)
    {
        for (int j = 0; j < image.width(); ++j)
        {
            int x = location - abs(j - location);

            ret(j, i, 0) = image(x, i, 0);
            ret(j, i, 1) = image(x, i, 1);
            ret(j, i, 2) = image(x, i, 2);
        }
    }

    return ret;
}

static Image mirror_along_y_axis(const Image& image, int location)
{
    Image ret = Image(image.width(), image.height());

    for (int i = 0; i < image.height(); ++i)
    {
        for (int j = 0; j < image.width(); ++j)
        {
            int y = location - abs(i - location);

            ret(j, i, 0) = image(j, y, 0);
            ret(j, i, 1) = image(j, y, 1);
            ret(j, i, 2) = image(j, y, 2);
        }
    }

    return ret;
}

static void sin_image(Float amplitude,
                      Float period,
                      Float phase,
                      Float offset,
                      Image& image,
                      ImagePartition part)
{
    switch(part)
    {
        case IP_X:
        {
            for (int j = 0; j < image.width(); ++j)
            {
                Float val = amplitude * std::sin(j / period + phase) + offset;

                for (int k = 0; k < image.depth(); ++k)
                {
                    for (int i = 0; i < image.height(); ++i)
                    {
                        image(j, i, k) = val;
                    }
                }
            }

            break;
        }
        case IP_Y:
        {
            for (int i = 0; i < image.height(); ++i)
            {
                Float val = amplitude * std::sin(i / period + phase) + offset;

                for (int k = 0; k < image.depth(); ++k)
                {
                    for (int j = 0; j < image.width(); ++j)
                    {
                        image(j, i, k) = val;
                    }
                }
            }

            break;
        }
        // case IP_Z:
        // {
        //     // TODO
        //     break;
        // }
        // case IP_R:
        // {
        //     // TODO
        //     break;
        // }
        // case IP_G:
        // {
        //     // TODO
        //     break;
        // }
        // case IP_B:
        // {
        //     // TODO
        //     break;
        // }
        // case IP_A:
        // {
        //     // TODO
        //     break;
        // }
        case IP_XY:
        {
            for (int i = 0; i < image.height(); ++i)
            {
                for (int j = 0; j < image.width(); ++j)
                {
                    // TODO: how should this be implemented to support different
                    //       periods for x and y
                    Float val = amplitude * std::sin((i + j) / period + phase) + offset;

                    for (int k = 0; k < image.depth(); ++k)
                    {
                        image(j, i, k) = val;
                    }
                }
            }

            break;
        }
        // case IP_XZ:
        // {
        //     // TODO
        //     break;
        // }
        // case IP_YZ:
        // {
        //     // TODO
        //     break;
        // }
        // case IP_XYZ:
        // {
        //     // TODO
        //     break;
        // }
        // case IP_RG:
        // {
        //     // TODO
        //     break;
        // }
        // case IP_RB:
        // {
        //     // TODO
        //     break;
        // }
        // case IP_RA:
        // {
        //     // TODO
        //     break;
        // }
        // case IP_GB:
        // {
        //     // TODO
        //     break;
        // }
        // case IP_GA:
        // {
        //     // TODO
        //     break;
        // }
        // case IP_BA:
        // {
        //     // TODO
        //     break;
        // }
        // case IP_RGB:
        // {
        //     // TODO
        //     break;
        // }
        // case IP_RGA:
        // {
        //     // TODO
        //     break;
        // }
        // case IP_RBA:
        // {
        //     // TODO
        //     break;
        // }
        // case IP_GBA:
        // {
        //     // TODO
        //     break;
        // }
        // case IP_RGBA:
        // {
        //     // TODO
        //     break;
        // }
    }
}

void perlin_flow_field(Image& image,
                       Float period,
                       int num_particles,
                       Float initial_speed = 1.0,
                       Float xtrans = 0.0,
                       Float ytrans = 0.0,
                       Float ztrans = 0.0)
{
    // TODO
}

}
