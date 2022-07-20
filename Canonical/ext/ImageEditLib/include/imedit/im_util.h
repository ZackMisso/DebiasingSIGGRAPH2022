/*
    ImageEditLib was developed by Zack Misso
    <zackary.t.misso.gr@dartmouth.edu>

    All code here is free to use as long as
    proper credit is given to the author
*/

// This file contains functional transformations from one image to another,
// as well as transformations from multiple images to a single image.

#pragma once

#include <imedit/image.h>
#include <vector>
#include <tgmath.h>

namespace imedit
{

static void clamp_im(Image& image)
{
    for (int i = 0; i < image.size(); ++i)
    {
        if (image[i] < 0.0) image[i] = 0.0;
        if (image[i] > 1.0) image[i] = 1.0;
    }
}

static void clamp_im(Image& image, Float min, Float max)
{
    for (int i = 0; i < image.size(); ++i)
    {
        if (image[i] < min) image[i] = min;
        if (image[i] > max) image[i] = max;
    }
}

static void apply_mask(Image& image,
                       const Image& mask,
                       Float threshold = 0.f,
                       Float scale = 0.f)
{
    for (int i = 0; i < image.size(); ++i)
    {
        if (mask[i] < 0.f)
        {
            image[i] = 0.f;
        }
        else if (mask[i] < threshold)
        {
            image[i] *= scale;
        }
    }
}

static Float clamp(Float val)
{
    if (val < 0.0) return 0.0;
    if (val > 1.0) return 1.0;
    return val;
}

static Float clamp(Float val, Float min, Float max)
{
    if (val < min) return min;
    if (val > max) return max;
    return val;
}

static void remap_range_lin(Image& image)
{
    Float min = image.min();
    Float max = image.max();

    for (int i = 0; i < image.size(); ++i)
    {
        image[i] = (image[i] - min) / (max - min);
    }
}

static void exp_im(Image& image, Float period)
{
    for (int i = 0; i < image.size(); ++i)
    {
        image[i] = exp(image[i] * period);
    }
}

static void pow_im(Image& image, Float exponent)
{
    for (int i = 0; i < image.size(); ++i)
    {
        image[i] = pow(image[i], exponent);
    }
}

static void remap_range_lin(std::vector<Image>& images)
{
    Float min = images[0].min();
    Float max = images[0].max();

    for (int i = 1; i < images.size(); ++i)
    {
        Float tmp_min = images[i].min();
        Float tmp_max = images[i].max();

        if (min > tmp_min) min = tmp_min;
        if (max < tmp_max) max = tmp_max;
    }

    for (int i = 0; i < images.size(); ++i)
    {
        for (int j = 0; j < images[i].size(); ++j)
        {
            images[i][j] = (images[i][j] - min) / (max - min);
        }
    }
}

static void remap_avg(Image& image, Float new_avg)
{
    Float avg = image.average();

    for (int i = 0; i < image.size(); ++i)
    {
        image[i] -= avg;
    }
}

// TODO: create tone map functionality

static void im_sin(Image& image)
{
    for (int i = 0; i < image.size(); ++i)
    {
        image[i] = sin(image[i]);
    }
}

static void im_cos(Image& image)
{
    for (int i = 0; i < image.size(); ++i)
    {
        image[i] = cos(image[i]);
    }
}

static void one_minus(Image& image)
{
    for (int i = 0; i < image.size(); ++i)
    {
        image[i] = (Float)1.0 - image[i];
    }
}

static void thresh_min(Image& image, Float threshold)
{
    for (int i = 0; i < image.size(); ++i)
    {
        if (image[i] < threshold) image[i] = threshold;
    }
}

static void thresh_min_zero(Image& image, Float threshold)
{
    for (int i = 0; i < image.size(); ++i)
    {
        if (image[i] < threshold) image[i] = 0.0;
    }
}

static void thresh_min_image(Image& image, Float threshold)
{
    for (int i = 0; i < image.size(); ++i)
    {
        if (image[i] > threshold) image[i] = 1.0;
        else image[i] = 0.0;
    }
}

static void thresh_max(Image& image, Float threshold)
{
    for (int i = 0; i < image.size(); ++i)
    {
        if (image[i] > threshold) image[i] = threshold;
    }
}

static void thresh_max_zero(Image& image, Float threshold)
{
    for (int i = 0; i < image.size(); ++i)
    {
        if (image[i] > threshold) image[i] = 0.0;
    }
}

static void thresh_max_image(Image& image, Float threshold)
{
    for (int i = 0; i < image.size(); ++i)
    {
        if (image[i] < threshold) image[i] = 1.0;
        else image[i] = 0.0;
    }
}

static void im_abs(Image& image)
{
    for (int i = 0; i < image.size(); ++i)
    {
        if (image[i] < 0.0) image[i] = -image[i];
    }
}

// TODO: why is this a pointer???
static Image* lerp(Float t, const Image& one, const Image& two)
{
    Image* img = new Image(one.width(), one.height());

    *img = one * (1.0 - t) + two * t;

    return img;
}

static void false_color_proxies(const Image& other,
                                std::vector<Float>& proxies,
                                Float min,
                                Float max)
{
    Float logmin = -std::log(min + 0.0000000001);
    Float logmax = -std::log(max + 0.0000000001);

    for (int i = 0; i < other.height(); ++i)
    {
        for (int j = 0; j < other.width(); ++j)
        {
            Float val = other(j, i, 0);

            Float logval = -std::log(val + 0.0000000001);

            proxies.push_back((logval - logmin) / (logmax - logmin));
        }
    }
}

// it is assumed the bins have already been given a size
// and that size is larger than 1
static void false_color_proxies_bins(const Image& other,
                                     std::vector<int>& bins,
                                     Float min,
                                     Float max,
                                     std::pair<Float, Float> range = std::pair<Float, Float>(0.0, 1.0))
{
    assert(bins.size() > 1);

    Float step = Float(1.0) / Float(bins.size() - 1);

    std::vector<Float> proxies = std::vector<Float>();

    false_color_proxies(other,
                        proxies,
                        min,
                        max);

    for (int i = 0; i < proxies.size(); ++i)
    {
        bins[std::floor((proxies[i] - range.first) / (range.second - range.first) / step)]++;
    }
}

// Note: this only applies a uniform color scaling
static void falseColorProxy(const Image& other,
                            Image& image,
                            Float min,
                            Float max,
                            std::pair<Float, Float> range = std::pair<Float, Float>(0.0, 1.0))
{
    for (int i = 0; i < other.height(); ++i)
    {
        for (int j = 0; j < other.width(); ++j)
        {
            // other is assumed to be black and white so o[0] == o[1] == o[2]
            Float val = other(j, i, 0);

            Float logmin = -std::log(min + 0.0000000001);
            Float logmax = -std::log(max + 0.0000000001);
            Float logval = -std::log(val + 0.0000000001);

            Float partial = (logval - logmin) / (logmax - logmin);

            Float proxy = partial;

            if (partial < range.first)
            {
                proxy = 0.0;
            }
            else if (partial > range.second)
            {
                proxy = 1.0;
            }
            else
            {
                proxy = (partial - range.first) / (range.second - range.first);
            }

            image(j, i, 0) = proxy;
            image(j, i, 1) = proxy;
            image(j, i, 2) = proxy;
        }
    }
}

static void histogram_grayscale(const Image& image,
                                std::vector<int>& hist,
                                Float min = 0.0,
                                Float max = 1.0)
{
    assert(hist.size() != 0);

    for (int i = 0; i < image.height(); ++i)
    {
        for (int j = 0; j < image.width(); ++j)
        {
            Float val = (image(j, i, 0) / (max - min)) * hist.size();

            int index = std::floor(val);

            if (index < 0) hist[0]++;
            else if (index >= hist.size()) hist[hist.size() - 1]++;
            else hist[index]++;
        }
    }
}

static Image edge_image(const Image& image)
{
    Image edge = Image(image.width(), image.height());

    for (int i = 1; i < edge.height()-1; ++i)
    {
        for (int j = 1; j < edge.width()-1; ++j)
        {
            edge(j, i, 0) = (image(j+1, i, 0) - image(j-1, i, 0) +
                            image(j, i+1, 0) - image(j, i-1, 0)) / 4.0;
            edge(j, i, 1) = (image(j+1, i, 1) - image(j-1, i, 1) +
                            image(j, i+1, 1) - image(j, i-1, 1)) / 4.0;
            edge(j, i, 2) = (image(j+1, i, 2) - image(j-1, i, 2) +
                            image(j, i+1, 2) - image(j, i-1, 2)) / 4.0;
        }
    }

    return edge;
}

static Image* low_avg_comparison(const std::vector<Image*>& images)
{
    double lowest = images[0]->average();
    Image* least_avg_image = images[0];

    for (int i = 1; i < images.size(); ++i)
    {
        double avg = images[i]->average();

        if (avg < lowest)
        {
            lowest = avg;
            least_avg_image = images[i];
        }
    }

    return least_avg_image;
}

static double mean_sqr_error(const Image& one, const Image& two)
{
    double err = 0.0;
    double size_term = 1.0 / one.size();

    for (int i = 0; i < one.size(); ++i)
    {
        err += (one[i] - two[i]) * (one[i] - two[i]);
    }

    err *= size_term;

    return err;
}

static double root_mean_sqr_error(const Image& one, const Image& two)
{
    return sqrt(mean_sqr_error(one, two));
}

static double mean_absolute_difference(const Image& one, const Image& two)
{
    double err = 0.0;
    double size_term = 1.0 / one.size();

    for (int i = 0; i < one.size(); ++i)
    {
        err += std::abs(one[i] - two[i]);
    }

    err *= size_term;

    return err;
}

static double mean_absolute_relative_difference(const Image& one, const Image& two)
{
    double err = 0.0;
    double size_term = 1.0 / one.size();

    for (int i = 0; i < one.size(); ++i)
    {
        err += std::abs(one[i] - two[i]) / one[i];
    }

    err *= size_term;

    return err;
}

static void hsl_to_rgb(Pixel& pixel)
{
    // hue is expected to be in the [0.0-1.0] range
    Float hue = pixel.r * 360.0;
    Float sat = pixel.g;
    Float lum = pixel.b;

    Float c = (1.0 - std::abs(2.0 * lum - 1.0)) * sat;
    Float x = c * (1.0 - std::abs(std::fmod((hue / 60.0), 2.0) - 1.0));
    Float m = lum - c / 2.0;

    Float rp = 0.0;
    Float gp = 0.0;
    Float bp = 0.0;

    if (hue >= 0.0 && hue < 60.0)
    {
        rp = c;
        gp = x;
    }
    else if (hue >= 60.0 && hue < 120.0)
    {
        rp = x;
        gp = c;
    }
    else if (hue >= 120.0 && hue < 180.0)
    {
        gp = c;
        bp = x;
    }
    else if (hue >= 180.0 && hue < 240.0)
    {
        gp = x;
        bp = c;
    }
    else if (hue >= 240.0 && hue < 300.0)
    {
        bp = c;
        rp = x;
    }
    else if (hue >= 300.0 && hue < 360.0)
    {
        bp = x;
        rp = c;
    }

    pixel.r = rp + m;
    pixel.g = gp + m;
    pixel.b = bp + m;
}

// TODO: this is very slow
static void minimize_neighbors(Image& image)
{
    bool minimized_any = true;

    while(minimized_any)
    {
        minimized_any = false;

        for (int i = image.height()-1; i >= 0; --i)
        {
            for (int j = image.width()-1; j >= 0; --j)
            {
                float old = image.safeAccess(j, i, 0);

                if (old <= 1.f)
                {
                    float min_val = fmin(
                        fmin(
                            image.safeAccess(j-1, i, 0),
                            image.safeAccess(j+1, i, 0)
                        ),
                        fmin(
                            image.safeAccess(j, i-1, 0),
                            image.safeAccess(j, i+1, 0)
                        )
                    );

                    if (min_val < old)
                    {
                        image(j, i, 0) = min_val;
                        image(j, i, 1) = min_val;
                        image(j, i, 2) = min_val;

                        minimized_any = true;
                    }
                }
            }
        }
    }
}

// this method assumes the input image has already been thresholded
static Image group_white(const Image& image, int& count)
{
    Image grouped = Image(image.width(), image.height());
    float max_val = image.width() * image.height() / 20;

    count = 1;
    float val = float(count) / max_val;

    for (int i = 0; i < image.height(); ++i)
    {
        for (int j = 0; j < image.width(); ++j)
        {
            if (image(j, i, 0) == 1.f)
            {
                float min_val = fmin(grouped.safeAccess(j-1, i, 0),
                                     grouped.safeAccess(j, i-1, 0));

                if (min_val > 0.1f)
                {
                    grouped(j, i, 0) = min_val;
                    grouped(j, i, 1) = min_val;
                    grouped(j, i, 2) = min_val;
                }
                else
                {
                    std::cout << count << std::endl;
                    grouped(j, i, 0) = val;
                    grouped(j, i, 1) = val;
                    grouped(j, i, 2) = val;
                    count++;
                    val = float(count) / max_val;
                }
            }
        }
    }

    // float max_val = image.width() * image.height();
    for (int i = 0; i < image.size(); ++i)
        if (grouped[i] < 1e-6)
            grouped[i] = 2.f;

    minimize_neighbors(grouped);

    for (int i = 0; i < image.size(); ++i)
        if (grouped[i] == 2.f)
            grouped[i] = 0.f;

    return grouped;
}

}
