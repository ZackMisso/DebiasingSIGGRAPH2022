#include <iostream>
#include <stdio.h>
#include "imedit/image.h"
#include "imedit/procedural.h"
#include "imedit/filter.h"
#include "imedit/im_util.h"
#include "pcg/pcg32.h"

typedef std::pair<float, float> Pff;
typedef imedit::Pixel Pix;

#define PI 3.14159265

int mask_timer = 50;

struct Location_Time
{
    Location_Time(double xpos, double ypos, int time)
        : xpos(xpos), ypos(ypos), time(time) { }

    double xpos;
    double ypos;
    int time;
};

struct KeyFrame
{
    KeyFrame(float xpos, float ypos, float time)
        : xpos(xpos), ypos(ypos), time(time) { }

    float xpos;
    float ypos;
    float time;
};

// TODO: make this a library function
void copy_image_if_pos_with_mask(imedit::Image& image, imedit::Image& mask, imedit::Image& copy, double cx, double cy)
{
    int initial_x = floor(cx - double(copy.width()) / 2.0);
    int initial_y = floor(cy - double(copy.height()) / 2.0);

    for (int x = initial_x; x < initial_x + copy.width(); ++x)
    {
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

// TODO: make this a library function
void check_mask(imedit::Image& image, imedit::Image& mask)
{
    mask -= 1.f;

    imedit::apply_mask(image, mask, 40.f, 0.985f);
}

void test_one()
{
    // initialize global parameters
    pcg32 rng = pcg32(0x4235, 0xac42);
    int res = 1024;
    int frames = 1000;
    std::string location = "displacement/";

    std::string rm_command = "rm -rf " + location;
    std::string mkdir_command = "mkdir " + location;

    std::string publish_command_1 = "ffmpeg -r 60 -f image2 -s 1024x1024 -i " + location + "/disp_%04d.png -vcodec libx264 -crf 25  -pix_fmt yuv420p " + location + "/test_2.mp4";
    std::string publish_command_2 = "ffmpeg -r 60 -f image2 -s 1024x1024 -i " + location + "/mask_%04d.png -vcodec libx264 -crf 25  -pix_fmt yuv420p " + location + "/test_2_mask.mp4";

    system(rm_command.c_str());
    system(mkdir_command.c_str());

    imedit::Image word_image_initial = imedit::Image("../data/ernie_ball.png");
    ~word_image_initial;

    imedit::Image word_image = imedit::Image(word_image_initial.width()*2, word_image_initial.height()*2, 3);

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

    word_image.write("initial_word.png");

    std::vector<Location_Time> time_stamps_one = std::vector<Location_Time>();
    std::vector<Location_Time> time_stamps_two = std::vector<Location_Time>();
    std::vector<Location_Time> time_stamps_three = std::vector<Location_Time>();
    std::vector<Location_Time> time_stamps_four = std::vector<Location_Time>();
    std::vector<Location_Time> time_stamps_five = std::vector<Location_Time>();

    float start_x = float(res) / 2.0 + word_image.width() / 2.0;
    float start_y = 2.75 * float(res) / 4.0;
    int start_t = 0;

    float end_x = float(res) / 2.0 - word_image.width() / 3.0;
    float end_y = 2.75 * float(res) / 4.0;
    int end_t = word_image.width() / 3;

    // std::cout << "end_T: " << end_t << std::endl;
    // std::cout << "last_T: " << last_t << std::endl;

    int last_t = 4 * word_image.width() / 2 + 60;

    end_t /= 2;

    int time_diff = 1002;
    time_diff = word_image.width() * 3;

    Pff last_one = Pff(cos(180.0 * PI / 180.0) * double(time_diff) + end_x, end_y - sin(180.0 * PI / 180.0) * double(time_diff));
    Pff last_two = Pff(cos(157.5 * PI / 180.0) * double(time_diff) + end_x, end_y - sin(157.5 * PI / 180.0) * double(time_diff));
    Pff last_three = Pff(cos(135.0 * PI / 180.0) * double(time_diff) + end_x, end_y - sin(135.0 * PI / 180.0) * double(time_diff));
    Pff last_four = Pff(cos(112.5 * PI / 180.0) * double(time_diff) + end_x, end_y - sin(112.5 * PI / 180.0) * double(time_diff));
    Pff last_five = Pff(cos(90.0 * PI / 180.0) * double(time_diff) + end_x, end_y - sin(90.0 * PI / 180.0) * double(time_diff));

    time_stamps_one.push_back(Location_Time(start_x, start_y, start_t));
    time_stamps_one.push_back(Location_Time(end_x, end_y, end_t));
    time_stamps_one.push_back(Location_Time(last_one.first, last_one.second, last_t));

    time_stamps_two.push_back(Location_Time(start_x, start_y, start_t));
    time_stamps_two.push_back(Location_Time(end_x, end_y, end_t));
    time_stamps_two.push_back(Location_Time(last_two.first, last_two.second, last_t));

    time_stamps_three.push_back(Location_Time(start_x, start_y, start_t));
    time_stamps_three.push_back(Location_Time(end_x, end_y, end_t));
    time_stamps_three.push_back(Location_Time(last_three.first, last_three.second, last_t));

    time_stamps_four.push_back(Location_Time(start_x, start_y, start_t));
    time_stamps_four.push_back(Location_Time(end_x, end_y, end_t));
    time_stamps_four.push_back(Location_Time(last_four.first, last_four.second, last_t));

    time_stamps_five.push_back(Location_Time(start_x, start_y, start_t));
    time_stamps_five.push_back(Location_Time(end_x, end_y, end_t));
    time_stamps_five.push_back(Location_Time(last_five.first, last_five.second, last_t));

    imedit::Image current_image = imedit::Image(res, res, 3);
    imedit::Image current_mask = imedit::Image(res, res, 3);

    int time_stamp = 1;

    for (int i = 0; i < current_mask.size(); ++i)
    {
        current_mask[i] = 1.0;
    }

    // TODO: this currently only works if all of the time stamps are the same
    for (int i = 0; i < frames; ++i)
    {
        std::cout << "creating frame: " << (i+1) << " out of: " << frames << " frames" << std::endl;

        current_image.clear();

        if (i > time_stamps_one[time_stamp].time && time_stamp < time_stamps_one.size())
        {
            time_stamp++;
        }

        if (time_stamp >= time_stamps_one.size())
        {
            // ONE
            {
                Location_Time stamp = time_stamps_one[time_stamps_one.size()-1];

                copy_image_if_pos_with_mask(current_image, current_mask, word_image, stamp.xpos, stamp.ypos);
            }

            // TWO
            {
                Location_Time stamp = time_stamps_two[time_stamps_two.size()-1];

                copy_image_if_pos_with_mask(current_image, current_mask, word_image, stamp.xpos, stamp.ypos);
            }

            // THREE
            {
                Location_Time stamp = time_stamps_three[time_stamps_three.size()-1];

                copy_image_if_pos_with_mask(current_image, current_mask, word_image, stamp.xpos, stamp.ypos);
            }

            // FOUR
            {
                Location_Time stamp = time_stamps_four[time_stamps_four.size()-1];

                copy_image_if_pos_with_mask(current_image, current_mask, word_image, stamp.xpos, stamp.ypos);
            }

            // FIVE
            {
                Location_Time stamp = time_stamps_five[time_stamps_five.size()-1];

                copy_image_if_pos_with_mask(current_image, current_mask, word_image, stamp.xpos, stamp.ypos);
            }
        }
        else
        {
            // ONE
            {
                Location_Time stamp_one = time_stamps_one[time_stamp-1];
                Location_Time stamp_two = time_stamps_one[time_stamp];

                double t = (stamp_two.time == stamp_one.time) ? 0.0 : double(i - stamp_one.time) / double(stamp_two.time - stamp_one.time);

                double xpos = stamp_two.xpos * t + (1.0 - t) * stamp_one.xpos;
                double ypos = stamp_two.ypos * t + (1.0 - t) * stamp_one.ypos;

                copy_image_if_pos_with_mask(current_image, current_mask, word_image, xpos, ypos);
            }

            // TWO
            {
                Location_Time stamp_one = time_stamps_two[time_stamp-1];
                Location_Time stamp_two = time_stamps_two[time_stamp];

                double t = (stamp_two.time == stamp_one.time) ? 0.0 : double(i - stamp_one.time) / double(stamp_two.time - stamp_one.time);

                double xpos = stamp_two.xpos * t + (1.0 - t) * stamp_one.xpos;
                double ypos = stamp_two.ypos * t + (1.0 - t) * stamp_one.ypos;

                copy_image_if_pos_with_mask(current_image, current_mask, word_image, xpos, ypos);
            }

            // THREE
            {
                Location_Time stamp_one = time_stamps_three[time_stamp-1];
                Location_Time stamp_two = time_stamps_three[time_stamp];

                double t = (stamp_two.time == stamp_one.time) ? 0.0 : double(i - stamp_one.time) / double(stamp_two.time - stamp_one.time);

                double xpos = stamp_two.xpos * t + (1.0 - t) * stamp_one.xpos;
                double ypos = stamp_two.ypos * t + (1.0 - t) * stamp_one.ypos;

                copy_image_if_pos_with_mask(current_image, current_mask, word_image, xpos, ypos);
            }

            // FOUR
            {
                Location_Time stamp_one = time_stamps_four[time_stamp-1];
                Location_Time stamp_two = time_stamps_four[time_stamp];

                double t = (stamp_two.time == stamp_one.time) ? 0.0 : double(i - stamp_one.time) / double(stamp_two.time - stamp_one.time);

                double xpos = stamp_two.xpos * t + (1.0 - t) * stamp_one.xpos;
                double ypos = stamp_two.ypos * t + (1.0 - t) * stamp_one.ypos;

                copy_image_if_pos_with_mask(current_image, current_mask, word_image, xpos, ypos);
            }

            // FIVE
            {
                Location_Time stamp_one = time_stamps_five[time_stamp-1];
                Location_Time stamp_two = time_stamps_five[time_stamp];

                double t = (stamp_two.time == stamp_one.time) ? 0.0 : double(i - stamp_one.time) / double(stamp_two.time - stamp_one.time);

                double xpos = stamp_two.xpos * t + (1.0 - t) * stamp_one.xpos;
                double ypos = stamp_two.ypos * t + (1.0 - t) * stamp_one.ypos;

                copy_image_if_pos_with_mask(current_image, current_mask, word_image, xpos, ypos);
            }
        }

        // check_mask(current_image, current_mask);

        current_image = mirror_along_x_axis(current_image, current_image.width() / 2);
        // current_mask = mirror_along_x_axis(current_mask, current_mask.width() / 2);

        current_image = mirror_along_y_axis(current_image, 4 * current_image.height() / 5);
        // current_mask = mirror_along_y_axis(current_mask, 4 * current_mask.height() / 5);

        char str[5];
        snprintf(str, 5, "%04d", i);

        // imedit::Image normalized_mask = current_mask / float(mask_timer);

        current_image.write(location + "disp_" + std::string(str) + ".png");
        // normalized_mask.write(location + "mask_" + std::string(str) + ".png");

        // imedit::Image backed_image = current_image * background;
        // imedit::Image backed_mask = normalized_mask * background;
        //
        // backed_image.write("adrien/backed_word_" + std::string(str) + ".png");
        // backed_mask.write("adrien/backed_mask_" + std::string(str) + ".png");
    }

    // TODO: how to organize this library of operations - should images be recomputed everytime or stored
    // as functions?

    system(publish_command_1.c_str());
    // system(publish_command_2.c_str());

    // imedit::Image mirrored = mirror_along_x_axis(word_image, word_image.width()/2);
    // mirrored.write("initial_mirrored.png");
}

struct Operator
{
    virtual ~Operator() { }

    // Pix evaluate_pixel(float x, float y)
    // {
    //     return Pix(evaluate(x, y, 0),
    //                evaluate(x, y, 1),
    //                evaluate(x, y, 2));
    // }

    virtual float evaluate(float val) const { return 0.f; }
    virtual Pff evaluate_location(float xpos, float ypos) const { return Pff(0.f, 0.f); }
    virtual Pix evaluate_color(float xpos, float ypos) { return Pix(); }

    imedit::Image evaluate_image(imedit::Image& image)
    {
        imedit::Image new_image = imedit::Image(image.width(), image.height(), 3);

        for (int i = 0; i < image.height(); ++i)
        {
            for (int j = 0; j < image.width(); ++j)
            {
                Pff loc = evaluate_location(j, i);
                int xpos = floor(loc.first);
                int ypos = floor(loc.second);

                new_image(j, i, 0) = image(xpos, ypos, 0);
                new_image(j, i, 1) = image(xpos, ypos, 1);
                new_image(j, i, 2) = image(xpos, ypos, 2);
            }
        }

        return new_image;
    }

    imedit::Image evaluate_image(imedit::Image& image,
                                 imedit::Image& mask,
                                 imedit::Image& word,
                                 const Operator* x_op,
                                 const Operator* y_op,
                                 int t)
    {
        imedit::Image new_image = imedit::Image(image.width(), image.height(), 3);

        for (int i = 0; i < image.height(); ++i)
        {
            for (int j = 0; j < image.width(); ++j)
            {
                Pff loc = evaluate_location(j, i);
                int xpos = floor(loc.first);
                int ypos = floor(loc.second);

                if (xpos != j && ypos != i)
                {
                    if (mask(j, i, 0) < 1.f)
                    {
                        xpos = floor(x_op->evaluate(0.25 * float(xpos) + t * 0.5));
                        ypos = floor(y_op->evaluate(0.5 * float(ypos)));

                        // std::cout << "xpos: " << xpos << " ypos: " << ypos << std::endl;

                        new_image(j, i, 0) = word(xpos, ypos, 0);
                        new_image(j, i, 1) = word(xpos, ypos, 1);
                        new_image(j, i, 2) = word(xpos, ypos, 2);

                        mask(j, i, 0) = 2.f;
                    }
                    else
                    {
                        new_image(j, i, 0) = image(j, i, 0);
                        new_image(j, i, 1) = image(j, i, 1);
                        new_image(j, i, 2) = image(j, i, 2);
                    }
                }
                else
                {
                    new_image(j, i, 0) = image(xpos, ypos, 0);
                    new_image(j, i, 1) = image(xpos, ypos, 1);
                    new_image(j, i, 2) = image(xpos, ypos, 2);
                }
            }
        }

        return new_image;
    }
};

struct SinOperator : public Operator
{
    SinOperator() { }

    virtual float evaluate(float val) const
    {

        // TODO
        return 0.f;
    }
};

struct LinOperator : public Operator
{
    LinOperator(float min, float max, float period, float start_t = 0.f)
        : min(min), max(max), period(period), start_t(start_t) { }

    virtual float evaluate(float val) const
    {
        if (val < start_t)
        {
            return 0.f;
        }

        float period_index = (val - start_t) / period;
        float interp = period_index - floor(period_index);
        return (max - min) * interp + min;
    }

    float min;
    float max;
    float period;
    float start_t;
};

struct MirrorOperator : public Operator
{
    MirrorOperator(Pff point, Pff tmp_dir, bool flip) : point(point), flip(flip)
    {
        // std::cout << tmp_dir.first << std::endl;
        // std::cout << tmp_dir.second << std::endl;
        dir.first = tmp_dir.first;
        dir.second = tmp_dir.second;

        // std::cout << dir.first << std::endl;
        // std::cout << dir.second << std::endl;

        float mag = sqrt(dir.first * dir.first + dir.second * dir.second);
        // std::cout << "mag: " << mag << std::endl;

        if (mag != 0.f)
        {
            // std::cout << "MAG: " << mag << std::endl;
            dir.first /= mag;
            dir.second /= mag;
        }

        float tmp = dir.first;
        dir.first = -dir.second;
        dir.second = tmp;

        if (flip)
        {
            dir.first = -dir.first;
            dir.second = -dir.second;
        }

        // std::cout << dir.first << std::endl;
        // std::cout << dir.second << std::endl;
    }

    // virtual void update_loc_and_dir(Pff point, Pff dir)
    // {
    //     dir.first = tmp_dir.first;
    //     dir.second = tmp_dir.second;
    //
    //     // std::cout << dir.first << std::endl;
    //     // std::cout << dir.second << std::endl;
    //
    //     float mag = sqrt(dir.first * dir.first + dir.second * dir.second);
    //     // std::cout << "mag: " << mag << std::endl;
    //     dir.first /= mag;
    //     dir.second /= mag;
    //
    //     float tmp = dir.first;
    //     dir.first = -dir.second;
    //     dir.second = tmp;
    // }

    virtual Pff evaluate_location(float xpos, float ypos) const
    {
        float p_m_x = xpos - point.first;
        float p_m_y = ypos - point.second;

        float dot = p_m_x * dir.first + p_m_y * dir.second;

        float proj_x = dir.first * dot;
        float proj_y = dir.second * dot;

        float norm_x = proj_x;
        float norm_y = proj_y;

        // if (flip && proj_y > 0.f)
        // {
        //     norm_x = -norm_x;
        //     norm_y = -norm_y;
        // }

        // std::cout << "xpos: " << xpos << std::endl;
        // // std::cout << "ypos: " << ypos << std::endl;
        // // // std::cout << std::endl;
        // // // std::cout << "p_x: " << point.first << std::endl;
        // // // std::cout << "p_y: " << point.second << std::endl;
        // // // std::cout << std::endl;
        // // std::cout << "p_m_x: " << p_m_x << std::endl;
        // // std::cout << "p_m_y: " << p_m_y << std::endl;
        // // // std::cout << std::endl;
        // std::cout << "norm_x: " << norm_x << std::endl;
        // // std::cout << "norm_y: " << norm_y << std::endl;
        // // // std::cout << std::endl;
        // // // std::cout << "proj_x: " << proj_x << std::endl;
        // // // std::cout << "proj_y: " << proj_y << std::endl;
        // // std::cout << std::endl;
        // // std::cout << "dir_x: " << dir.first << std::endl;
        // // std::cout << "dir_y: " << dir.second << std::endl;
        // // std::cout << std::endl;
        // //
        // // float decide_dot = dir.first * p_m_x + dir.second * p_m_y;
        // // std::cout << "decision: " << decide_dot << std::endl;

        if (dot > 0.f)
        {
            // dont mirror
            return Pff(xpos, ypos);
        }
        else
        {
            // mirror
            return Pff(xpos - 2.f * norm_x, ypos - 2.f * norm_y);
        }

        // float proj_p_x = proj_x - p_m_x;
        // float proj_p_y = proj_y - p_m_y;
        //
        // if (flip && proj_p_y > 0.f)
        // {
        //     proj_p_y = -proj_p_y;
        // }
        //
        // return Pff(0.f, 0.f);
    }

    Pff point;
    Pff dir;
    bool flip;
};

struct TwoPointMirrorOperator : public Operator
{
    TwoPointMirrorOperator(Pff point_one, Pff point_two, bool flip)
        : point_one(point_one), point_two(point_two), flip(flip)
    {
        update_dir();
    }

    void update_point_one(Pff point)
    {
        // std::cout << "OLD One: " << point_one.first << " " << point_one.second << std::endl;
        point_one.first = point.first;
        point_one.second = point.second;
        // std::cout << "NEW One: " << point_one.first << " " << point_one.second << std::endl;
        // std::cout << std::endl;
        update_dir();
    }

    void update_point_two(Pff point)
    {
        // std::cout << "OLD Two: " << point_two.first << " " << point_two.second << std::endl;
        point_two.first = point.first;
        point_two.second = point.second;
        // std::cout << "NEW Two: " << point_two.first << " " << point_two.second << std::endl;
        update_dir();
    }

    void update_dir()
    {
        dir.first = point_one.first - point_two.first;
        dir.second = point_one.second - point_two.second;

        float mag = sqrt(dir.first * dir.first + dir.second * dir.second);

        if (mag != 0.f)
        {
            // std::cout << "MAG: " << mag << std::endl;
            dir.first /= mag;
            dir.second /= mag;
        }

        float tmp = dir.first;
        dir.first = -dir.second;
        dir.second = tmp;

        if (flip)
        {
            dir.first = -dir.first;
            dir.second = -dir.second;
        }
    }

    virtual Pff evaluate_location(float xpos, float ypos) const
    {
        if (dir.first == 0.f && dir.second == 0.f)
            return Pff(xpos, ypos);

        // std::cout << "dir.x: " << dir.first << " dir.y: " << dir.second << std::endl;

        float p_m_x = xpos - point_one.first;
        float p_m_y = ypos - point_one.second;

        float dot = p_m_x * dir.first + p_m_y * dir.second;

        // std::cout << "dot: " << dot << std::endl;
        // std::cout << "pmx: " << p_m_x << " pmy: " << p_m_y << std::endl;

        float proj_x = dir.first * dot;
        float proj_y = dir.second * dot;

        // std::cout << dir.first << " " << dir.second << std::endl;

        float norm_x = proj_x;
        float norm_y = proj_y;

        if (dot > 0.f)
        {
            // dont mirror
            return Pff(xpos, ypos);
        }
        else
        {
            // std::cout << "wat" << std::endl;
            // mirror
            return Pff(xpos - 2.f * norm_x, ypos - 2.f * norm_y);
        }
    }

    void debug()
    {
        std::cout << "one: " << point_one.first << " " << point_one.second << std::endl;
        std::cout << "two: " << point_two.first << " " << point_two.second << std::endl;
    }

    Pff point_one;
    Pff point_two;
    Pff dir;
    bool flip;
};

struct AngleMirrorOperator : public Operator
{
    AngleMirrorOperator(Pff point, float angle, bool flip, Operator* last = nullptr)
        : point(point), angle(angle), flip(flip), last(last)
    {
        update_dir();
    }

    void update_point(Pff p)
    {
        point.first = p.first;
        point.second = p.second;
        update_dir();
    }

    void update_angle(float a)
    {
        if (angle < 180.0 && a >= 180.0)
            flip = !flip;

        angle = a;
        update_dir();
    }

    void update_dir()
    {
        dir.first = cos(PI / 180.0 * angle);
        dir.second = sin(PI / 180.0 * angle);

        float tmp = dir.first;
        dir.first = -dir.second;
        dir.second = tmp;

        if (flip)
        {
            dir.first = -dir.first;
            dir.second = -dir.second;
        }
    }

    virtual Pff evaluate_location(float xpos, float ypos) const
    {
        if (last)
        {
            Pff new_loc = last->evaluate_location(xpos, ypos);
            xpos = new_loc.first;
            ypos = new_loc.second;
        }

        if (dir.first == 0.f && dir.second == 0.f)
            return Pff(xpos, ypos);

        // std::cout << "dir.x: " << dir.first << " dir.y: " << dir.second << std::endl;

        float p_m_x = xpos - point.first;
        float p_m_y = ypos - point.second;

        float dot = p_m_x * dir.first + p_m_y * dir.second;

        // std::cout << "dot: " << dot << std::endl;
        // std::cout << "pmx: " << p_m_x << " pmy: " << p_m_y << std::endl;

        float proj_x = dir.first * dot;
        float proj_y = dir.second * dot;

        // std::cout << dir.first << " " << dir.second << std::endl;

        float norm_x = proj_x;
        float norm_y = proj_y;

        if (dot > 0.f)
        {
            // dont mirror
            return Pff(xpos, ypos);
        }
        else
        {
            // std::cout << "wat" << std::endl;
            // mirror
            return Pff(xpos - 2.f * norm_x, ypos - 2.f * norm_y);
        }
    }

    void debug()
    {
        std::cout << "point: " << point.first << " " << point.second << std::endl;
        std::cout << "angle: " << angle << std::endl;
    }

    Operator* last;
    Pff point;
    Pff dir;
    float angle;
    bool flip;
};

struct CombineTwoOperator : Operator
{
    CombineTwoOperator(Operator* x_op, Operator* y_op) : x_op(x_op), y_op(y_op) { }

    virtual Pff evaluate_location(float xpos, float ypos) const
    {
        xpos = 0.25 * xpos + t * 0.5;
        ypos = 0.5 * ypos;
        return Pff(x_op->evaluate(xpos), y_op->evaluate(ypos));
    }

    void update(float t_param)
    {
        t = t_param;
    }

    Operator* x_op;
    Operator* y_op;
    float t;
};

std::vector<Pff> get_points(int index, int res)
{
    std::vector<Pff> points = std::vector<Pff>();

    // if (index == 0)
    // {
    //     // squre
    //     points.push_back(Pff(0, res));
    //     points.push_back(Pff(0, 0));
    //     points.push_back(Pff(res, 0));
    //     points.push_back(Pff(res, 0));
    //     points.push_back(Pff(res, 0));
    // }
    // else if (index == 1)
    // {
    //     // diamond
    //     points.push_back(Pff(res/2, res));
    //     points.push_back(Pff(0, res/2));
    //     points.push_back(Pff(res/2, 0));
    //     points.push_back(Pff(res/2, 0));
    //     points.push_back(Pff(res/2, 0));
    // }
    // else if (index == 2)
    // {
    //     // pentagon
    //     points.push_back(Pff(res/2, res));
    //     points.push_back(Pff(0, 2*res/3));
    //     points.push_back(Pff(res/4, 0));
    //     points.push_back(Pff(res/2, 0));
    //     points.push_back(Pff(res/2, 0));
    // }
    // else if (index == 3)
    // {
    //     // hexagon
    //     points.push_back(Pff(res/2, res));
    //     points.push_back(Pff(0, 3*res/4));
    //     points.push_back(Pff(0, res/4));
    //     points.push_back(Pff(res/4, 0));
    //     points.push_back(Pff(res/2, 0));
    // }
    // else if (index == 4)
    // {
    //     // death
    //     points.push_back(Pff(res/2, res));
    //     points.push_back(Pff(0, res));
    //     points.push_back(Pff(0, 0));
    //     points.push_back(Pff(0, 0));
    //     points.push_back(Pff(res/2, 0));
    // }

    // std::cout << float(res/2) * cos(PI / 180.0 * 90.0) << std::endl;
    // std::cout << float(res/2) * sin(PI / 180.0 * 90.0) + float(res/2) << std::endl;

    if (index == 0)
    {
        points.push_back(Pff(90.f, 90.f));
        points.push_back(Pff(90.f, 90.f));
        points.push_back(Pff(90.f, 90.f));
        points.push_back(Pff(90.f, 90.f));
        points.push_back(Pff(90.f, 90.f));
        points.push_back(Pff(90.f, 90.f));
        points.push_back(Pff(90.f, 90.f));
        points.push_back(Pff(90.f, 90.f));
        points.push_back(Pff(90.f, 90.f));
        points.push_back(Pff(90.f, 90.f));
    }
    else if (index == 1)
    {
        points.push_back(Pff(90 + 90.f * 0.5f / 1.f, 90.f * 0.5f / 1.f));
        points.push_back(Pff(90.f, 90.f));
        points.push_back(Pff(90.f, 90.f));
        points.push_back(Pff(90.f, 90.f));
        points.push_back(Pff(90.f, 90.f));
        points.push_back(Pff(90.f, 90.f));
        points.push_back(Pff(90.f, 90.f));
        points.push_back(Pff(90.f, 90.f));
        points.push_back(Pff(90.f, 90.f));
        points.push_back(Pff(90.f, 90.f));
    }
    else if (index == 2)
    {
        points.push_back(Pff(90 + 90.f * 1.5f / 2.f, 90.f * 1.5f / 2.f));
        points.push_back(Pff(90 + 90.f * 0.5f / 2.f, 90.f * 0.5f / 2.f));
        points.push_back(Pff(90.f, 90.f));
        points.push_back(Pff(90.f, 90.f));
        points.push_back(Pff(90.f, 90.f));
        points.push_back(Pff(90.f, 90.f));
        points.push_back(Pff(90.f, 90.f));
        points.push_back(Pff(90.f, 90.f));
        points.push_back(Pff(90.f, 90.f));
        points.push_back(Pff(90.f, 90.f));
    }
    else if (index == 3)
    {
        points.push_back(Pff(90 + 90.f * 2.5f / 3.f, 90.f * 2.5f / 3.f));
        points.push_back(Pff(90 + 90.f * 1.5f / 3.f, 90.f * 1.5f / 3.f));
        points.push_back(Pff(90 + 90.f * 0.5f / 3.f, 90.f * 0.5f / 3.f));
        points.push_back(Pff(90.f, 90.f));
        points.push_back(Pff(90.f, 90.f));
        points.push_back(Pff(90.f, 90.f));
        points.push_back(Pff(90.f, 90.f));
        points.push_back(Pff(90.f, 90.f));
        points.push_back(Pff(90.f, 90.f));
        points.push_back(Pff(90.f, 90.f));
    }
    else if (index == 4)
    {
        points.push_back(Pff(90 + 90.f * 3.5f / 4.f, 90.f * 3.5f / 4.f));
        points.push_back(Pff(90 + 90.f * 2.5f / 4.f, 90.f * 2.5f / 4.f));
        points.push_back(Pff(90 + 90.f * 1.5f / 4.f, 90.f * 1.5f / 4.f));
        points.push_back(Pff(90 + 90.f * 0.5f / 4.f, 90.f * 0.5f / 4.f));
        points.push_back(Pff(90.f, 90.f));
        points.push_back(Pff(90.f, 90.f));
        points.push_back(Pff(90.f, 90.f));
        points.push_back(Pff(90.f, 90.f));
        points.push_back(Pff(90.f, 90.f));
        points.push_back(Pff(90.f, 90.f));
    }
    else if (index == 5)
    {
        points.push_back(Pff(90 + 90.f * 4.5f / 5.f, 90.f * 4.5f / 5.f));
        points.push_back(Pff(90 + 90.f * 3.5f / 5.f, 90.f * 3.5f / 5.f));
        points.push_back(Pff(90 + 90.f * 2.5f / 5.f, 90.f * 2.5f / 5.f));
        points.push_back(Pff(90 + 90.f * 1.5f / 5.f, 90.f * 1.5f / 5.f));
        points.push_back(Pff(90 + 90.f * 0.5f / 5.f, 90.f * 0.5f / 5.f));
        points.push_back(Pff(90.f, 90.f));
        points.push_back(Pff(90.f, 90.f));
        points.push_back(Pff(90.f, 90.f));
        points.push_back(Pff(90.f, 90.f));
        points.push_back(Pff(90.f, 90.f));
    }
    else if (index == 6)
    {
        points.push_back(Pff(90 + 90.f * 5.5f / 6.f, 90.f * 5.5f / 6.f));
        points.push_back(Pff(90 + 90.f * 4.5f / 6.f, 90.f * 4.5f / 6.f));
        points.push_back(Pff(90 + 90.f * 3.5f / 6.f, 90.f * 3.5f / 6.f));
        points.push_back(Pff(90 + 90.f * 2.5f / 6.f, 90.f * 2.5f / 6.f));
        points.push_back(Pff(90 + 90.f * 1.5f / 6.f, 90.f * 1.5f / 6.f));
        points.push_back(Pff(90 + 90.f * 0.5f / 6.f, 90.f * 0.5f / 6.f));
        points.push_back(Pff(90.f, 90.f));
        points.push_back(Pff(90.f, 90.f));
        points.push_back(Pff(90.f, 90.f));
        points.push_back(Pff(90.f, 90.f));
    }
    else if (index == 7)
    {
        points.push_back(Pff(90 + 90.f * 6.5f / 7.f, 90.f * 6.5f / 7.f));
        points.push_back(Pff(90 + 90.f * 5.5f / 7.f, 90.f * 5.5f / 7.f));
        points.push_back(Pff(90 + 90.f * 4.5f / 7.f, 90.f * 4.5f / 7.f));
        points.push_back(Pff(90 + 90.f * 3.5f / 7.f, 90.f * 3.5f / 7.f));
        points.push_back(Pff(90 + 90.f * 2.5f / 7.f, 90.f * 2.5f / 7.f));
        points.push_back(Pff(90 + 90.f * 1.5f / 7.f, 90.f * 1.5f / 7.f));
        points.push_back(Pff(90 + 90.f * 0.5f / 7.f, 90.f * 0.5f / 7.f));
        points.push_back(Pff(90.f, 90.f));
        points.push_back(Pff(90.f, 90.f));
        points.push_back(Pff(90.f, 90.f));
    }
    else if (index == 8)
    {
        points.push_back(Pff(90 + 90.f * 7.5f / 8.f, 90.f * 7.5f / 8.f));
        points.push_back(Pff(90 + 90.f * 6.5f / 8.f, 90.f * 6.5f / 8.f));
        points.push_back(Pff(90 + 90.f * 5.5f / 8.f, 90.f * 5.5f / 8.f));
        points.push_back(Pff(90 + 90.f * 4.5f / 8.f, 90.f * 4.5f / 8.f));
        points.push_back(Pff(90 + 90.f * 3.5f / 8.f, 90.f * 3.5f / 8.f));
        points.push_back(Pff(90 + 90.f * 2.5f / 8.f, 90.f * 2.5f / 8.f));
        points.push_back(Pff(90 + 90.f * 1.5f / 8.f, 90.f * 1.5f / 8.f));
        points.push_back(Pff(90 + 90.f * 0.5f / 8.f, 90.f * 0.5f / 8.f));
        points.push_back(Pff(90.f, 90.f));
        points.push_back(Pff(90.f, 90.f));
    }
    else if (index == 9)
    {
        points.push_back(Pff(90 + 90.f * 8.5f / 9.f, 90.f * 8.5f / 9.f));
        points.push_back(Pff(90 + 90.f * 7.5f / 9.f, 90.f * 7.5f / 9.f));
        points.push_back(Pff(90 + 90.f * 6.5f / 9.f, 90.f * 6.5f / 9.f));
        points.push_back(Pff(90 + 90.f * 5.5f / 9.f, 90.f * 5.5f / 9.f));
        points.push_back(Pff(90 + 90.f * 4.5f / 9.f, 90.f * 4.5f / 9.f));
        points.push_back(Pff(90 + 90.f * 3.5f / 9.f, 90.f * 3.5f / 9.f));
        points.push_back(Pff(90 + 90.f * 2.5f / 9.f, 90.f * 2.5f / 9.f));
        points.push_back(Pff(90 + 90.f * 1.5f / 9.f, 90.f * 1.5f / 9.f));
        points.push_back(Pff(90 + 90.f * 0.5f / 9.f, 90.f * 0.5f / 9.f));
        points.push_back(Pff(90.f, 90.f));
    }

    return points;
}

Pff interp_points(Pff start, Pff end, float t)
{
    // std::cout << "T: " << t << std::endl;
    // std::cout << "x: " << end.first * t + start.first * (1.f - t) << std::endl;
    // std::cout << "y: " << end.second * t + start.second * (1.f - t) << std::endl;
    return Pff(end.first * t + start.first * (1.f - t),
               end.second * t + start.second * (1.f - t));
}

void test_two()
{
    // initialize global parameters
    pcg32 rng = pcg32(0x4235, 0xac42);
    int res = 1024;
    int frames = 400;
    std::string location = "displacement/";

    std::string rm_command = "rm -rf " + location;
    std::string mkdir_command = "mkdir " + location;

    std::string publish_command_1 = "ffmpeg -r 60 -f image2 -s 1024x1024 -i " + location + "/disp_%04d.png -vcodec libx264 -crf 25  -pix_fmt yuv420p " + location + "/test_5.mp4";
    std::string publish_command_2 = "ffmpeg -r 60 -f image2 -s 1024x1024 -i " + location + "/mask_%04d.png -vcodec libx264 -crf 25  -pix_fmt yuv420p " + location + "/test_5_mask.mp4";

    system(rm_command.c_str());
    system(mkdir_command.c_str());

    imedit::Image word_image = imedit::Image("../data/ernie_ball.png");
    ~word_image;

    LinOperator x_operator = LinOperator(0, word_image.width(), 100, 0);
    LinOperator y_operator = LinOperator(0, word_image.height(), 100, 0);

    // MirrorOperator y_mirror = MirrorOperator(Pff(res / 2, 0), Pff(0, 1), false);
    // MirrorOperator bl_mirror = MirrorOperator(Pff(res / 2, res), Pff(-1, -1), false);
    // MirrorOperator tl_mirror = MirrorOperator(Pff(res / 2, 0), Pff(-1, 1), true);

    TwoPointMirrorOperator y_mirror = TwoPointMirrorOperator(Pff(res / 2, 0), Pff(res / 2, res), true);
    // TwoPointMirrorOperator bl_mirror = TwoPointMirrorOperator(Pff(0, res / 2), Pff(res / 2, res), false);
    // TwoPointMirrorOperator tl_mirror = TwoPointMirrorOperator(Pff(res / 2, 0), Pff(0, res / 2), false);

    TwoPointMirrorOperator bl_mirror = TwoPointMirrorOperator(Pff(res, 0), Pff(0, 0), false);
    TwoPointMirrorOperator tl_mirror = TwoPointMirrorOperator(Pff(res, 0), Pff(0, 0), false);

    TwoPointMirrorOperator mirror_one = TwoPointMirrorOperator(Pff(0, res), Pff(0, 0), false);
    TwoPointMirrorOperator mirror_two = TwoPointMirrorOperator(Pff(0, 0), Pff(res, 0), false);
    TwoPointMirrorOperator mirror_three = TwoPointMirrorOperator(Pff(0, 0), Pff(res, 0), false);
    TwoPointMirrorOperator mirror_four = TwoPointMirrorOperator(Pff(0, 0), Pff(res, 0), false);
    // TwoPointMirrorOperator mirror_five = TwoPointMirrorOperator(Pff(res, 0), Pff(0, 0), false);
    // TwoPointMirrorOperator mirror_six = TwoPointMirrorOperator(Pff(res, 0), Pff(0, 0), false);
    // TwoPointMirrorOperator mirror_seven = TwoPointMirrorOperator(Pff(res, 0), Pff(0, 0), false);
    // TwoPointMirrorOperator mirror_eight = TwoPointMirrorOperator(Pff(res, 0), Pff(0, 0), false);
    // TwoPointMirrorOperator mirror_nine = TwoPointMirrorOperator(Pff(res, 0), Pff(0, 0), false);
    // TwoPointMirrorOperator mirror_ten = TwoPointMirrorOperator(Pff(res, 0), Pff(0, 0), false);

    // LinOperator bl_mirror_timer_x_one = LinOperator(res, , 400, 0);
    // LinOperator bl_mirror_timer_y_one = LinOperator(90.0, 225.0, 400, 0);
    // LinOperator tl_mirror_timer_x_one = LinOperator(90.0, 225.0, 400, 0);
    // LinOperator tl_mirror_timer_y_one = LinOperator(90.0, 225.0, 400, 0);
    // LinOperator tl_mirror_timer = LinOperator(90.0, 135.0, 400, 400.f * 2.f / 3.f);

    imedit::Image current_image = imedit::Image(res, res, 3);

    // TODO: this currently only works if all of the time stamps are the same
    for (int t = 0; t < frames; ++t)
    {
        std::cout << "creating frame: " << (t+1) << " out of: " << frames << " frames" << std::endl;

        current_image.clear();

        for (int i = 0; i < current_image.height(); ++i)
        {
            for (int j = 0; j < current_image.width(); ++j)
            {
                // TODO: change to float
                int xpos = floor(x_operator.evaluate(0.25 * float(j) + t * 0.5));
                int ypos = floor(y_operator.evaluate(0.5 * float(i)));

                current_image(j, i, 0) = word_image(xpos, ypos, 0);
                current_image(j, i, 1) = word_image(xpos, ypos, 1);
                current_image(j, i, 2) = word_image(xpos, ypos, 2);
            }
        }

        float interp = 0.f;
        int index_last = 0;
        int index_next = 0;

        if (t < 200)
        {
            interp = float(t) / 200.f;
            index_last = 0;
            index_next = 1;
        }
        else if (t < 300)
        {
            interp = 1.f;
            index_last = 0;
            index_next = 1;
        }
        else if (t < 400)
        {
            interp = float(t-300) / float(100);
            index_last = 1;
            index_next = 2;
        }
        else if (t < 500)
        {
            interp = 1.f;
            index_last = 1;
            index_next = 2;
        }
        else if (t < 600)
        {
            interp = float(t-500) / float(100);
            index_last = 2;
            index_next = 3;
        }
        else if (t < 700)
        {
            interp = 1.f;
            index_last = 2;
            index_next = 3;
        }
        else if (t < 770)
        {
            interp = float(t-700) / float(70);
            index_last = 3;
            index_next = 4;
        }
        else if (t < 850)
        {
            interp = 1.f;
            index_last = 3;
            index_next = 4;
        }

        std::vector<Pff> points_before = get_points(index_last, res);
        std::vector<Pff> points_after = get_points(index_next, res);

        mirror_one.update_point_two(interp_points(points_before[0],
                                                  points_after[0],
                                                  interp));

        mirror_one.update_point_one(interp_points(points_before[1],
                                                  points_after[1],
                                                  interp));

        mirror_two.update_point_two(interp_points(points_before[1],
                                                  points_after[1],
                                                  interp));

        mirror_two.update_point_one(interp_points(points_before[2],
                                                  points_after[2],
                                                  interp));

        mirror_three.update_point_two(interp_points(points_before[2],
                                                    points_after[2],
                                                    interp));

        mirror_three.update_point_one(interp_points(points_before[3],
                                                    points_after[3],
                                                    interp));

        mirror_four.update_point_two(interp_points(points_before[3],
                                                   points_after[3],
                                                   interp));

        mirror_four.update_point_one(interp_points(points_before[4],
                                                   points_after[4],
                                                   interp));
        // else if (t < 400)
        // {
        //     float interp = float(t - 200) / float(200);
        //     bl_mirror.update_point_two(Pff(interp * float(res) + (1.f - interp) * 0, res));
        //     bl_mirror.update_point_one(Pff(0, interp * float(res) + (1.f - interp) * 0));
        //
        //     tl_mirror.update_point_two(Pff(0, interp * float(res) + (1.f - interp) * 0));
        //     tl_mirror.update_point_one(Pff(interp * 0.f + (1.f - interp) * float(res), 0));
        // }

        // if (t < 200)
        // {
        //     float interp = float(t) / float(200);
        //
        //     mirror_two.update_point_two(Pff(0, interp * float(res/2) + (1.f - interp) * 0));
        //     mirror_two.update_point_one(Pff(interp * float(res/2) + (1.f - interp) * float(res), 0));
        //
        //     mirror_one.update_point_two(Pff(interp * float(res/2) + (1.f - interp) * 0, res));
        //     mirror_one.update_point_one(Pff(0, interp * float(res/2) + (1.f - interp) * 0));
        // }
        // else if (t < 300 && t > 230)
        // {
        //     float interp = float(t - 230) / float(70);
        //
        //     float dist = res / 3.f;
        //
        //     mirror_three.update_point_two(Pff(0, interp * float(res/3) + (1.f - interp) * 0));
        //     mirror_three.update_point_one(Pff(interp * float(res) + (1.f - interp) * float(res/2), 0));
        //
        //     mirror_two.update_point_two(Pff(0, interp * float(2*res/3) + (1.f - interp) * float(res/2)));
        //     mirror_two.update_point_one(Pff(interp * float(res/3) + (1.f - interp) * float(res/2), 0));
        //
        //     mirror_one.update_point_two(Pff(interp * float(2*res/3) + (1.f - interp) * float(res/2), res));
        //     mirror_one.update_point_one(Pff(0, interp * float(2*res/3) + (1.f - interp) * float(res/2)));
        // }

        current_image = mirror_one.evaluate_image(current_image);
        current_image = mirror_two.evaluate_image(current_image);
        current_image = mirror_three.evaluate_image(current_image);
        current_image = mirror_four.evaluate_image(current_image);
        // current_image = mirror_five.evaluate_image(current_image);
        // current_image = mirror_six.evaluate_image(current_image);
        // current_image = mirror_seven.evaluate_image(current_image);
        // current_image = mirror_eight.evaluate_image(current_image);
        // current_image = mirror_nine.evaluate_image(current_image);
        // current_image = mirror_ten.evaluate_image(current_image);
        current_image = y_mirror.evaluate_image(current_image);


        // OLD THING //
        // for (int i = 0; i < current_image.height(); ++i)
        // {
        //     for (int j = 0; j < current_image.width(); ++j)
        //     {
        //         // TODO: change to float
        //         int xpos = floor(x_operator.evaluate(0.25 * float(j) + t * 0.5));
        //         int ypos = floor(y_operator.evaluate(0.5 * float(i)));
        //
        //         current_image(j, i, 0) = word_image(xpos, ypos, 0);
        //         current_image(j, i, 1) = word_image(xpos, ypos, 1);
        //         current_image(j, i, 2) = word_image(xpos, ypos, 2);
        //     }
        // }
        //
        // if (t < 200)
        // {
        //     float interp = float(t) / float(200);
        //     bl_mirror.update_point_two(Pff(0, interp * float(res) + (1.f - interp) * 0));
        //     bl_mirror.update_point_one(Pff(interp * 0.f + (1.f - interp) * float(res), 0));
        // }
        // else if (t < 400)
        // {
        //     float interp = float(t - 200) / float(200);
        //     bl_mirror.update_point_two(Pff(interp * float(res) + (1.f - interp) * 0, res));
        //     bl_mirror.update_point_one(Pff(0, interp * float(res) + (1.f - interp) * 0));
        //
        //     tl_mirror.update_point_two(Pff(0, interp * float(res) + (1.f - interp) * 0));
        //     tl_mirror.update_point_one(Pff(interp * 0.f + (1.f - interp) * float(res), 0));
        // }
        //
        // current_image = bl_mirror.evaluate_image(current_image);
        // current_image = tl_mirror.evaluate_image(current_image);
        // current_image = y_mirror.evaluate_image(current_image);

        // current_image = mirror_along_x_axis(current_image, current_image.width() / 2);
        // current_image
        // current_image = mirror_along_y_axis(current_image, 4 * current_image.height() / 5);
        //\\ OLD THING \\//

        char str[5];
        snprintf(str, 5, "%04d", 849 - t);

        current_image.write(location + "disp_" + std::string(str) + ".png");
    }

    system(publish_command_1.c_str());
}

void test_three()
{
    // initialize global parameters
    pcg32 rng = pcg32(0x4235, 0xac42);
    int res = 1024;
    int frames = 2000;
    std::string location = "displacement_final_fox/";

    std::string rm_command = "rm -rf " + location;
    std::string mkdir_command = "mkdir " + location;

    std::string publish_command_1 = "ffmpeg -r 60 -f image2 -s 1024x1024 -i " + location + "/disp_%04d.png -vcodec libx264 -crf 25  -pix_fmt yuv420p " + location + "/test_5.mp4";
    std::string publish_command_2 = "ffmpeg -r 60 -f image2 -s 1024x1024 -i " + location + "/mask_%04d.png -vcodec libx264 -crf 25  -pix_fmt yuv420p " + location + "/test_5_mask.mp4";

    system(rm_command.c_str());
    system(mkdir_command.c_str());

    // imedit::Image word_image = imedit::Image("../data/ernie_ball.png");
    // ~word_image;

    imedit::Image fox_image = imedit::Image("../data/final_fox_4.png");
    fox_image.alterGamma(1.f / 2.2f, 1.0f);
    imedit::Image word_image = imedit::Image(fox_image.width(), fox_image.height(), 3);

    for (int i = 0; i < fox_image.height(); ++i)
    {
        for (int j = 0; j < fox_image.width(); ++j)
        {
            word_image(j, i, 0) = fox_image(fox_image.width() - j - 1, i, 0);
            word_image(j, i, 1) = fox_image(fox_image.width() - j - 1, i, 1);
            word_image(j, i, 2) = fox_image(fox_image.width() - j - 1, i, 2); 
        }
    }

    LinOperator x_operator = LinOperator(0, word_image.width(), 100, 0);
    LinOperator y_operator = LinOperator(0, word_image.height(), 100, 0);

    CombineTwoOperator combine_operator = CombineTwoOperator(&x_operator, &y_operator);

    // MirrorOperator y_mirror = MirrorOperator(Pff(res / 2, 0), Pff(0, 1), false);
    // MirrorOperator bl_mirror = MirrorOperator(Pff(res / 2, res), Pff(-1, -1), false);
    // MirrorOperator tl_mirror = MirrorOperator(Pff(res / 2, 0), Pff(-1, 1), true);

    TwoPointMirrorOperator y_mirror = TwoPointMirrorOperator(Pff(res / 2, 0), Pff(res / 2, res), true);

    // AngleMirrorOperator mirror_ten = AngleMirrorOperator(Pff(res/2, 0), 90.f, true);
    // AngleMirrorOperator mirror_nine = AngleMirrorOperator(Pff(res/2, 0), 90.f, true);
    // AngleMirrorOperator mirror_eight = AngleMirrorOperator(Pff(res/2, 0), 90.f, true);
    // AngleMirrorOperator mirror_seven = AngleMirrorOperator(Pff(res/2, 0), 90.f, true);
    // AngleMirrorOperator mirror_six = AngleMirrorOperator(Pff(res/2, 0), 90.f, true);
    // AngleMirrorOperator mirror_five = AngleMirrorOperator(Pff(res/2, 0), 90.f, true);
    // AngleMirrorOperator mirror_four = AngleMirrorOperator(Pff(res/2, 0), 90.f, true, &combine_operator);
    // AngleMirrorOperator mirror_three = AngleMirrorOperator(Pff(res/2, 0), 90.f, true, &mirror_four);
    // AngleMirrorOperator mirror_one = AngleMirrorOperator(Pff(res/2, 0), 90.f, true, &mirror_one);
    AngleMirrorOperator mirror_one = AngleMirrorOperator(Pff(res/2, 0), 90.f, true);
    AngleMirrorOperator mirror_two = AngleMirrorOperator(Pff(res/2, 0), 90.f, true, &mirror_one);
    AngleMirrorOperator mirror_three = AngleMirrorOperator(Pff(res/2, 0), 90.f, true, &mirror_two);
    AngleMirrorOperator mirror_four = AngleMirrorOperator(Pff(res/2, 0), 90.f, true, &mirror_three);
    AngleMirrorOperator mirror_five = AngleMirrorOperator(Pff(res/2, 0), 90.f, true, &mirror_four);
    AngleMirrorOperator mirror_six = AngleMirrorOperator(Pff(res/2, 0), 90.f, true, &mirror_five);
    AngleMirrorOperator mirror_seven = AngleMirrorOperator(Pff(res/2, 0), 90.f, true, &mirror_six);
    AngleMirrorOperator mirror_eight = AngleMirrorOperator(Pff(res/2, 0), 90.f, true, &mirror_seven);
    AngleMirrorOperator mirror_nine = AngleMirrorOperator(Pff(res/2, 0), 90.f, true, &mirror_eight);
    AngleMirrorOperator mirror_ten = AngleMirrorOperator(Pff(res/2, 0), 90.f, true, &mirror_nine);
    // AngleMirrorOperator mirror_one = AngleMirrorOperator(Pff(res/2, 0), 90.f, true, &mirror_two);

    imedit::Image current_image = imedit::Image(res, res, 3);
    imedit::Image current_mask = imedit::Image(res, res, 3);

    // TODO: this currently only works if all of the time stamps are the same
    for (int t = 0; t < frames; ++t)
    {
        std::cout << "creating frame: " << (t+1) << " out of: " << frames << " frames" << std::endl;

        current_image.clear();
        current_mask.clear();

        float interp = 0.f;
        int index_last = 0;
        int index_next = 0;

        if (t < 200)
        {
            interp = float(t) / 200.f;
            index_last = 0;
            index_next = 1;
        }
        else if (t < 300)
        {
            interp = 1.f;
            index_last = 0;
            index_next = 1;
        }
        else if (t < 400)
        {
            interp = float(t-300) / float(100);
            index_last = 1;
            index_next = 2;
        }
        else if (t < 500)
        {
            interp = 1.f;
            index_last = 1;
            index_next = 2;
        }
        else if (t < 600)
        {
            interp = float(t-500) / float(100);
            index_last = 2;
            index_next = 3;
        }
        else if (t < 700)
        {
            interp = 1.f;
            index_last = 2;
            index_next = 3;
        }
        else if (t < 800)
        {
            interp = float(t-700) / float(100);
            index_last = 3;
            index_next = 4;
        }
        else if (t < 900)
        {
            interp = 1.f;
            index_last = 3;
            index_next = 4;
        }
        else if (t < 1000)
        {
            interp = float(t-900) / float(100);
            index_last = 4;
            index_next = 5;
        }
        else if (t < 1100)
        {
            interp = 1.f;
            index_last = 4;
            index_next = 5;
        }
        else if (t < 1200)
        {
            interp = float(t-1100) / float(100);
            index_last = 5;
            index_next = 6;
        }
        else if (t < 1300)
        {
            interp = 1.f;
            index_last = 5;
            index_next = 6;
        }
        else if (t < 1400)
        {
            interp = float(t-1300) / float(100);
            index_last = 6;
            index_next = 7;
        }
        else if (t < 1500)
        {
            interp = 1.f;
            index_last = 6;
            index_next = 7;
        }
        else if (t < 1600)
        {
            interp = float(t-1500) / float(100);
            index_last = 7;
            index_next = 8;
        }
        else if (t < 1700)
        {
            interp = 1.f;
            index_last = 7;
            index_next = 8;
        }
        else if (t < 1800)
        {
            interp = float(t-1700) / float(100);
            index_last = 8;
            index_next = 9;
        }
        else if (t < 2000)
        {
            interp = 1.f;
            index_last = 8;
            index_next = 9;
        }

        std::vector<Pff> points_before = get_points(index_last, res);
        std::vector<Pff> points_after = get_points(index_next, res);

        mirror_one.update_angle(interp_points(points_before[0],
                                              points_after[0],
                                              interp).first);

        mirror_two.update_angle(interp_points(points_before[1],
                                              points_after[1],
                                              interp).first);

        mirror_three.update_angle(interp_points(points_before[2],
                                                points_after[2],
                                                interp).first);

        mirror_four.update_angle(interp_points(points_before[3],
                                               points_after[3],
                                               interp).first);

        mirror_five.update_angle(interp_points(points_before[4],
                                               points_after[4],
                                               interp).first);

        mirror_six.update_angle(interp_points(points_before[5],
                                              points_after[5],
                                              interp).first);

        mirror_seven.update_angle(interp_points(points_before[6],
                                                points_after[6],
                                                interp).first);

        mirror_eight.update_angle(interp_points(points_before[7],
                                                points_after[7],
                                                interp).first);

        mirror_nine.update_angle(interp_points(points_before[8],
                                               points_after[8],
                                               interp).first);

        mirror_ten.update_angle(interp_points(points_before[9],
                                              points_after[9],
                                              interp).first);

        // mirror_two.update_point_two(interp_points(points_before[1],
        //                                           points_after[1],
        //                                           interp));
        //
        // mirror_two.update_point_one(interp_points(points_before[2],
        //                                           points_after[2],
        //                                           interp));
        //
        // mirror_three.update_point_two(interp_points(points_before[2],
        //                                             points_after[2],
        //                                             interp));
        //
        // mirror_three.update_point_one(interp_points(points_before[3],
        //                                             points_after[3],
        //                                             interp));
        //
        // mirror_four.update_point_two(interp_points(points_before[3],
        //                                            points_after[3],
        //                                            interp));
        //
        // mirror_four.update_point_one(interp_points(points_before[4],
        //                                            points_after[4],
        //                                            interp));

        // current_image = mirror_ten.evaluate_image(current_image,
        //                                           word_image,
        //                                           &x_operator,
        //                                           &y_operator,
        //                                           t);
        //
        // current_image = mirror_nine.evaluate_image(current_image,
        //                                           word_image,
        //                                           &x_operator,
        //                                           &y_operator,
        //                                           t);
        //
        // current_image = mirror_eight.evaluate_image(current_image,
        //                                             word_image,
        //                                             &x_operator,
        //                                             &y_operator,
        //                                             t);
        //
        // current_image = mirror_seven.evaluate_image(current_image,
        //                                            word_image,
        //                                            &x_operator,
        //                                            &y_operator,
        //                                            t);
        //
        // current_image = mirror_six.evaluate_image(current_image,
        //                                            word_image,
        //                                            &x_operator,
        //                                            &y_operator,
        //                                            t);
        //
        // current_image = mirror_five.evaluate_image(current_image,
        //                                           word_image,
        //                                           &x_operator,
        //                                           &y_operator,
        //                                           t);
        //
        // current_image = mirror_four.evaluate_image(current_image,
        //                                             word_image,
        //                                             &x_operator,
        //                                             &y_operator,
        //                                             t);
        //
        // current_image = mirror_three.evaluate_image(current_image,
        //                                             word_image,
        //                                             &x_operator,
        //                                             &y_operator,
        //                                             t);
        //
        // current_image = mirror_two.evaluate_image(current_image,
        //                                           current_mask,
        //                                           word_image,
        //                                           &x_operator,
        //                                           &y_operator,
        //                                           t);
        //
        // current_image = mirror_one.evaluate_image(current_image,
        //                                           current_mask,
        //                                           word_image,
        //                                           &x_operator,
        //                                           &y_operator,
        //                                           t);

        // mirror_one.debug();

        for (int i = 0; i < current_image.height(); ++i)
        {
            for (int j = 0; j < current_image.width(); ++j)
            {
                combine_operator.update(t);
                Pff location = mirror_ten.evaluate_location(j, i);
                // location = combine_operator.evaluate_location(location.first, location.second);
                // int xpos = floor(location.first);
                // int ypos = floor(location.second);

                // std::cout << "j: " << j << " xpos: " << xpos << std::endl;
                // std::cout << "i: " << i << " ypos: " << ypos << std::endl;


                // TODO: change to float
                int xpos = floor(x_operator.evaluate(0.25 * float(location.first) + t * 0.5));
                int ypos = floor(y_operator.evaluate(0.5 * float(location.second)));

                current_image(j, i, 0) = word_image(xpos, ypos, 0);
                current_image(j, i, 1) = word_image(xpos, ypos, 1);
                current_image(j, i, 2) = word_image(xpos, ypos, 2);
            }
        }

        current_image = y_mirror.evaluate_image(current_image);

        char str[5];
        snprintf(str, 5, "%04d", t);

        current_image.write(location + "disp_" + std::string(str) + ".png");
        // current_mask.write(location + "mask_" + std::string(str) + ".png");
    }

    system(publish_command_1.c_str());
}

int main(int argc, char* argv[])
{
    // test_one();
    // test_two();
    test_three();

    return 0;
}
