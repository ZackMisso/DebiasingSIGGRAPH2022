#include <iostream>
#include <stdio.h>
#include "imedit/image.h"
#include "imedit/procedural.h"
#include "imedit/filter.h"
#include "imedit/im_util.h"
#include "pcg/pcg32.h"

typedef std::pair<double, double> Pff;
typedef imedit::Pixel Pix;

int mask_timer = 100;

struct Location_Time
{
    Location_Time(double xpos, double ypos, int time)
        : xpos(xpos), ypos(ypos), time(time) { }

    double xpos;
    double ypos;
    int time;
};

void copy_image(imedit::Image& image, imedit::Image& copy, double cx, double cy)
{
    int initial_x = floor(cx - double(copy.width()) / 2.0);
    int initial_y = floor(cy - double(copy.height()) / 2.0);

    for (int y = initial_y; y < initial_y + copy.height(); ++y)
    {
        for (int x = initial_x; x < initial_x + copy.width(); ++x)
        {
            if (x >= 0 && x < image.width() &&
                y >= 0 && y < image.height())
            {
                image(x, y, 0) = copy(x - initial_x, y - initial_y, 0);
                image(x, y, 1) = copy(x - initial_x, y - initial_y, 1);
                image(x, y, 2) = copy(x - initial_x, y - initial_y, 2);
            }
        }
    }
}

void copy_image_if_pos(imedit::Image& image, imedit::Image& copy, double cx, double cy)
{
    int initial_x = floor(cx - double(copy.width()) / 2.0);
    int initial_y = floor(cy - double(copy.height()) / 2.0);

    for (int y = initial_y; y < initial_y + copy.height(); ++y)
    {
        for (int x = initial_x; x < initial_x + copy.width(); ++x)
        {
            if (x >= 0 && x < image.width() &&
                y >= 0 && y < image.height())
            {
                if (copy(x - initial_x, y - initial_y, 0) > 0.0)
                    image(x, y, 0) = copy(x - initial_x, y - initial_y, 0);
                if (copy(x - initial_x, y - initial_y, 1) > 0.0)
                    image(x, y, 1) = copy(x - initial_x, y - initial_y, 1);
                if (copy(x - initial_x, y - initial_y, 2) > 0.0)
                    image(x, y, 2) = copy(x - initial_x, y - initial_y, 2);
            }
        }
    }
}

void copy_image_if_pos_with_mask(imedit::Image& image, imedit::Image& mask, imedit::Image& copy, double cx, double cy)
{
    int initial_x = floor(cx - double(copy.width()) / 2.0);
    int initial_y = floor(cy - double(copy.height()) / 2.0);

    // std::cout << image.width() << " blah " << image.height() << std::endl;
    // std::cout << copy.width() << " " << copy.height() << std::endl;

    // std::cout << "x: " << initial_x << " y: " << initial_y << std::endl;

    // std::cout << "bob: " << initial_x << " " << initial_x + copy.width() << std::endl;

    for (int x = initial_x; x < initial_x + copy.width(); ++x)
    {
        // std::cout << " x: " << x << std::endl;
        for (int y = initial_y; y < initial_y + copy.height(); ++y)
        {
            if (x >= 0 && x < image.width() &&
                y >= 0 && y < image.height())
            {
                if (x - initial_x < 0 || x - initial_x >= copy.width()) std::cout << "FUCK" << std::endl;
                if (y - initial_y < 0 || y - initial_y >= copy.height()) std::cout << "FUCK" << std::endl;
                if (copy(x - initial_x, y - initial_y, 0) > 0.0)
                {
                    image(x, y, 0) = copy(x - initial_x, y - initial_y, 0);
                    mask(x, y, 0) = mask_timer;
                }
                if (copy(x - initial_x, y - initial_y, 1) > 0.0)
                {
                    image(x, y, 1) = copy(x - initial_x, y - initial_y, 1);
                    mask(x, y, 1) = mask_timer;
                }
                if (copy(x - initial_x, y - initial_y, 2) > 0.0)
                {
                    image(x, y, 2) = copy(x - initial_x, y - initial_y, 2);
                    mask(x, y, 2) = mask_timer;
                }
            }
        }
    }
}

void check_mask(imedit::Image& image, imedit::Image& mask)
{
    for (int i = 0; i < image.size(); ++i)
    {
        mask[i] -= 1.f;

        if (mask[i] < 0.f)
        {
            image[i] = 0.f;
        }
        else if (mask[i] < 80.f)
        {
            image[i] *= 0.985;
        }
    }
}

imedit::Image create_background()
{
    double res = 1024.0;
    int num_points = 90;

    imedit::Image image = imedit::Image(1024, 1024, 3);

    pcg32 rng(0xacd213, 0x23856);

    std::vector<Pff> points = std::vector<Pff>();
    std::vector<Pix> colors = std::vector<Pix>();

    double sat = 0.7;
    double lum = 0.4;
    double minh = 0.2;
    double maxh = 0.35;

    for (int i = 0; i < num_points; ++i)
    {
        points.push_back(Pff(rng.nextFloat() * res, rng.nextFloat() * res));
        // random hue sampling
        Pix color = Pix((maxh - minh) * rng.nextFloat() + minh, sat, lum);
        // uniform hue sampling
        // Pix color = Pix((maxh - minh) * (float(i) / float(num_points-1)) + minh, sat, lum);
        imedit::hsl_to_rgb(color);
        colors.push_back(color);
    }

    imedit::euclidean_tiled_image(image, points, colors);

    imedit::Image edge_image = imedit::Image(image.width(), image.height(), 3);
    imedit::euclidean_edge_image(edge_image, points);
    edge_image *= 0.6;
    // ~edge_image;
    copy_image_if_pos(image, edge_image, 512.0, 512.0);

    imedit::Image fresnel_image = imedit::Image(image.width(), image.height(), 3);
    imedit::euclidean_dist_image(fresnel_image, points);
    fresnel_image /= fresnel_image.max();
    imedit::pow_im(fresnel_image, 2.5);
    // ~fresnel_image;

    imedit::Image noise_image = imedit::Image(image.width(), image.height(), 3);
    imedit::noise_image_xy(noise_image, 1.0);
    imedit::clamp_im(noise_image);
    noise_image *= 0.2;

    for (int i = 0; i < fresnel_image.size(); ++i)
    {
        if (fresnel_image[i] < 0.9 && fresnel_image[i] > 0.2)
        {
            fresnel_image[i] -= noise_image[i] * image[i];
        }
    }
    // imedit::clamp_im(fresnel_image);

    image = image * fresnel_image * 0.5 + image * 0.5;

    // imedit::Image edge_image = imedit::edge_image(image);
    // edge_image *= 4.0;

    image.write("background.png");
    edge_image.write("edge.png");
    fresnel_image.write("fresnel.png");

    return image;
}

int main(int argc, char* argv[])
{
    imedit::Image background = create_background();

    std::string location = "adrien";

    std::string rm_command = "rm -rf " + location;
    std::string mkdir_command = "mkdir " + location;
    std::string publish_command = "ffmpeg -r 60 -f image2 -s 1024x1024 -i " + location + "/word_%04d.png -vcodec libx264 -crf 25  -pix_fmt yuv420p " + location + "/test_5.mp4";
    std::string publish_command_2 = "ffmpeg -r 60 -f image2 -s 1024x1024 -i " + location + "/mask_%04d.png -vcodec libx264 -crf 25  -pix_fmt yuv420p " + location + "/test_5_mask.mp4";
    std::string publish_command_3 = "ffmpeg -r 60 -f image2 -s 1024x1024 -i " + location + "/backed_word_%04d.png -vcodec libx264 -crf 25  -pix_fmt yuv420p " + location + "/test_5_backed.mp4";
    std::string publish_command_4 = "ffmpeg -r 60 -f image2 -s 1024x1024 -i " + location + "/backed_mask_%04d.png -vcodec libx264 -crf 25  -pix_fmt yuv420p " + location + "/test_5_backed_mask.mp4";

    system(rm_command.c_str());
    system(mkdir_command.c_str());

    imedit::Image word_image_initial = imedit::Image("../data/ernie_ball.png");
    ~word_image_initial;

    // std::cout << "w: " << word_image_initial.width() << std::endl;

    imedit::Image word_image = imedit::Image(word_image_initial.width()*2, word_image_initial.height()*2, 3);

    // std::cout << "w: " << word_image.width() << std::endl;

    for (int i = 0; i < word_image_initial.height(); ++i)
    {
        for (int j = 0; j < word_image_initial.width(); ++j)
        {
            word_image(2*j, 2*i, 0) = word_image_initial(j, i, 0);
            word_image(2*j, 2*i, 1) = word_image_initial(j, i, 1);
            word_image(2*j, 2*i, 2) = word_image_initial(j, i, 2);

            word_image(2*j+1, 2*i, 0) = word_image_initial(j, i, 0);
            word_image(2*j+1, 2*i, 1) = word_image_initial(j, i, 1);
            word_image(2*j+1, 2*i, 2) = word_image_initial(j, i, 2);

            word_image(2*j, 2*i+1, 0) = word_image_initial(j, i, 0);
            word_image(2*j, 2*i+1, 1) = word_image_initial(j, i, 1);
            word_image(2*j, 2*i+1, 2) = word_image_initial(j, i, 2);

            word_image(2*j+1, 2*i+1, 0) = word_image_initial(j, i, 0);
            word_image(2*j+1, 2*i+1, 1) = word_image_initial(j, i, 1);
            word_image(2*j+1, 2*i+1, 2) = word_image_initial(j, i, 2);
        }
    }

    word_image *= 2.f;
    imedit::clamp_im(word_image);

    // word_image.write("test.png");

    std::vector<Location_Time> time_stamps = std::vector<Location_Time>();

    time_stamps.push_back(Location_Time(512.0, 1500.0, 0));

    // std::cout << "initial: " << time_stamps[0].xpos << std::endl;
    // std::cout << "word_image: " << word_image.width() << std::endl;
    // std::cout << time_stamps.size() << std::endl;

    int time = 20;

    double amplitude = 512.0;

    for(int i = 0; i < 180; ++i)
    {
        time += 10;
        time_stamps.push_back(Location_Time(512.0 + amplitude * cos(3.1415 / 2.0 - double(i) / 20.0 * 3.1415),
                                            512.0 + amplitude * (sin(3.1415 / 2.0 - double(i) / 20.0 * 3.1415)),
                                            time));

        if (i > 135)
            amplitude *= 0.95;
        else
            amplitude *= 0.99;
    }

    // time += 30;
    // time_stamps.push_back(Location_Time(512.0, 512.0, time));

    int frames = time + 300;
    int time_stamp = 1;

    // frames = 665;

    imedit::Image current_image = imedit::Image(1024, 1024, 3);
    imedit::Image current_mask = imedit::Image(1024, 1024, 3);

    for (int i = 0; i < current_mask.size(); ++i)
    {
        current_mask[i] = 1.0;
    }

    current_image.write("initial_image.png");

    for (int i = 0; i < frames; ++i)
    {
        std::cout << "creating frame: " << (i+1) << " out of: " << frames << " frames" << std::endl;

        if (i > time_stamps[time_stamp].time && time_stamp < time_stamps.size())
        {
            time_stamp++;
        }

        if (time_stamp >= time_stamps.size())
        {
            Location_Time stamp = time_stamps[time_stamps.size()-1];

            copy_image_if_pos_with_mask(current_image, current_mask, word_image, stamp.xpos, stamp.ypos);
        }
        else
        {
            // std::cout << "time_stamp: " << time_stamp << std::endl;
            Location_Time stamp_one = time_stamps[time_stamp-1];
            Location_Time stamp_two = time_stamps[time_stamp];

            double t = (stamp_two.time == stamp_one.time) ? 0.0 : double(i - stamp_one.time) / double(stamp_two.time - stamp_one.time);

            // std::cout << "t: " << t << std::endl;
            // std::cout << "onex: " << stamp_one.xpos << " twox: " << stamp_two.xpos << std::endl;
            // std::cout << "oney: " << stamp_one.ypos << " twoy: " << stamp_two.ypos << std::endl;

            double xpos = stamp_two.xpos * t + (1.0 - t) * stamp_one.xpos;
            double ypos = stamp_two.ypos * t + (1.0 - t) * stamp_one.ypos;

            // std::cout << "xpos: " << xpos << " ypos: " << ypos << std::endl;

            copy_image_if_pos_with_mask(current_image, current_mask, word_image, xpos, ypos);
        }

        check_mask(current_image, current_mask);

        char str[5];
        snprintf(str, 5, "%04d", i);

        imedit::Image normalized_mask = current_mask / float(mask_timer);

        current_image.write("adrien/word_" + std::string(str) + ".png");
        normalized_mask.write("adrien/mask_" + std::string(str) + ".png");

        imedit::Image backed_image = current_image * background;
        imedit::Image backed_mask = normalized_mask * background;

        backed_image.write("adrien/backed_word_" + std::string(str) + ".png");
        backed_mask.write("adrien/backed_mask_" + std::string(str) + ".png");
    }

    system(publish_command.c_str());
    system(publish_command_2.c_str());
    system(publish_command_3.c_str());
    system(publish_command_4.c_str());

    return 0;
}
