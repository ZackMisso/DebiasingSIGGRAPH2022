#include <iostream>
#include "imedit/image.h"
#include "imedit/procedural.h"
#include "imedit/im_color_maps.h"

#define PI 3.1415269

float tri(float val)
{

}

void init_vert_lines(imedit::Image& image)
{
    for (int i = 0; i < image.height(); ++i)
    {
        for (int j = 0; j < image.width(); ++j)
        {
            float val = 0.5 * (sin(0.05 * float(j)) + 1.0);

            image(j, i, 0) = val;
            image(j, i, 1) = val;
            image(j, i, 2) = val;
        }
    }
}

float cauchy(float x0, float x, float gamma)
{
    float sq = (x - x0) / gamma;
    sq *= sq;
    float div = PI * gamma * (1.0 + sq);
    return 1.0 / div;
}

void init_vert_lines_2(imedit::Image& image)
{
    for (int i = 0; i < image.height(); ++i)
    {
        for (int j = 0; j < image.width(); ++j)
        {
            float val = 0.5 * (sin(float(j) / 100.0) + 1.0) -
                               10.0 * cauchy(100, j, 1) -
                               10.0 * cauchy(200, j, 1) -
                               10.0 * cauchy(800, j, 1);

            if (val < 0.3) val = 0.0;
            else val = 1.0;

            image(j, i, 0) = val;
            image(j, i, 1) = val;
            image(j, i, 2) = val;
        }
    }
}

float flat_bound(float proxy, float min, float max)
{
    if (proxy > max) return 0.0;
    if (proxy < min) return 0.0;
    return 1.0;
}

float cheat(int j, int i)
{
    // mid = 256
    // sin(256) = 1

    float proxy = sin(float(i) * 3.1415269 / (2.0 * 256.0));

    // std::cout << sin(256.0 * 3.1415269 / (2.0 * 256.0)) << std::endl;

    // height of 50
    if (j < std::abs(100 * flat_bound(i, 206.f, 306.f) *
            cos(std::abs(256 - i) * 0.031)))
    {
        // std::cout << 100 + std::abs(200 * sin(proxy)) << std::endl;
        return 0.0f;
    }
    if (j < std::abs(230 * cos(std::abs(256 - i) * 0.012)) *
            flat_bound(i, 126.f, 386.f))
    {
        return 0.5f;
    }
    if (j < std::abs(1256 * cos(std::abs(256 - i) * 0.005)) *
            flat_bound(i, 0.f, 512.f))
    {
        return 0.0f;
    }
    return 0.5f;
}

float image_eval(float x)
{
    float val = 0.5 * (sin(x / 100.0) + 1.0) -
                0.5 * cauchy(0.2, std::abs(sin(float(x) / 100.0)), 0.2);

    // val = 0.5 * (sin(x / 100.0) + 1.0);

    // val = 0.5 * cauchy(0.2, std::abs(sin(float(x) / 100.0)), 0.2);
    return val;


    // for (int i = 0; i < image.height(); ++i)
    // {
    //     for (int j = 0; j < image.width(); ++j)
    //     {
    //         float val = 0.5 * (sin(float(j) / 100.0) + 1.0) -
    //                            10.0 * cauchy(100, j, 1) -
    //                            10.0 * cauchy(200, j, 1) -
    //                            10.0 * cauchy(800, j, 1);
    //
    //         if (val < 0.3) val = 0.0;
    //         else val = 1.0;
    //
    //         image(j, i, 0) = val;
    //         image(j, i, 1) = val;
    //         image(j, i, 2) = val;
    //     }
    // }
}

imedit::Image displace_top(const imedit::Image& input)
{
    imedit::Image image = imedit::Image(input.width(), input.height(), 3);

    for (int i = 0; i < input.height()/2; ++i)
    {
        for (int j = 0; j < input.width(); ++j)
        {
            // int in_x = int(std::max(expf(0.01 * j), float(input.width())));
            // int in_x = 100 * 0.5 * (sin(cheat(j)) + 1.0) + 48.0;
            int in_x = 100 * cheat(j, i) + 48.0;

            image(j, i, 0) = input(in_x, i, 0);
            image(j, i, 1) = input(in_x, i, 1);
            image(j, i, 2) = input(in_x, i, 2);
        }
    }

    for (int i = input.height()/2; i < input.height(); ++i)
    {
        for (int j = 0; j < input.width(); ++j)
        {
            image(j, i, 0) = input(j, i, 0);
            image(j, i, 1) = input(j, i, 0);
            image(j, i, 2) = input(j, i, 0);
        }
    }

    return image;
}

void flip(imedit::Image& image, int width, int j, int i)
{
    for (int k = 0; k < width; ++k)
    {
        image(j + width + k, i, 0) = image(j - k, i, 0);
        image(j + width + k, i, 1) = image(j - k, i, 1);
        image(j + width + k, i, 2) = image(j - k, i, 2);
    }
}

void rev_flip(imedit::Image& image, int width, int j, int i)
{
    for (int k = 0; k < width; ++k)
    {
        image(j - (width + k), i, 0) = image(j + k, i, 0);
        image(j - (width + k), i, 1) = image(j + k, i, 1);
        image(j - (width + k), i, 2) = image(j + k, i, 2);
    }
}

//
// imedit::Image jitter_xaxis(const imedit::Image& input)
// {
//     imedit::Image image = imedit::Image(input.width(), input.height(), 3);
//
//
//
//     for (int i = 0; i < input.height(); ++i)
//     {
//         for (int j = 0; j < input.width(); ++j)
//         {
//             image(j, i, 0) = input(j, i, 0);
//             image(j, i, 1) = input(j, i, 0);
//             image(j, i, 2) = input(j, i, 0);
//         }
//     }
//
//     for (int i = 0; i < input.height()/2; ++i)
//     {
//         for (int j = 0; j < input.width()-1; ++j)
//         {
//             float proxy = sin(float(i) * 3.1415269 / (2.0 * 256.0));
//             int width = 40 * proxy;
//
//             if (image(j, i, 0) != image(j+1, i, 0) &&
//                 image(j, i, 0) < 0.1)
//             {
//                 flip(image, width, j, i);
//                 j += width * 2;
//             }
//             // else
//             // {
//             //     rev_flip(image, width, j, i);
//             // }
//         }
//     }
//
//     // for (int i = input.height()/2; i < input.height(); ++i)
//     // {
//     //     for (int j = 0; j < input.width(); ++j)
//     //     {
//     //         image(j, i, 0) = input(j, i, 0);
//     //         image(j, i, 1) = input(j, i, 0);
//     //         image(j, i, 2) = input(j, i, 0);
//     //     }
//     // }
//
//     return image;
// }

int main(int argc, char* argv[])
{
    std::string publish_command = "ffmpeg -r 60 -f image2 -i " "wavy/frame_%03d.png -vcodec mpeg4 -vb 20M -minrate 20M -maxrate 30M wavy/wavy.mp4";

    for (int k = 0; k < 0; ++k)
    {
        std::cout << "rendering frame: " << k << std::endl;
        imedit::Image image = imedit::Image(1024, 1024, 3);

        float amp = float(k) / 1000.0 * 0.031;

        for (int i = 0; i < image.height(); ++i)
        {
            for (int j = 0; j < image.width(); ++j)
            {
                float val = image_eval(j * cos(std::abs(256 - i) * amp));
                // float val = image_eval(j);
                image(j, i, 0) = val;
                image(j, i, 1) = val;
                image(j, i, 2) = val;
            }
        }

        // init_vert_lines(image);
        // image.write("wavy/wavy_base.png");
        // image = displace_top(image);
        // image.write("wavy/take_1.png");
        // image = jitter_xaxis(image);
        // image.write("wavy/take_2.png");

        char str[4];
        snprintf(str, 4, "%03d", k);

        image.write("wavy/frame_" + std::string(str) + ".png");

        // init_vert_lines_2(image);
        // image.write("wavy/wavy_base_6.png");
    }

    system(publish_command.c_str());

    return 0;
}
