#include <iostream>
#include "imedit/image.h"
#include "imedit/procedural.h"
#include "imedit/filter.h"
#include "imedit/im_util.h"
#include "pcg/pcg32.h"

typedef std::pair<double, double> Pff;
typedef imedit::Pixel Pix;

void initialize_random_points(std::vector<Pff>& pts,
                              int num_tile_spots,
                              float min_bounds,
                              float max_bounds,
                              pcg32& rng)
{
    pts = std::vector<Pff>(num_tile_spots);

    for (int i = 0; i < num_tile_spots; ++i)
    {
        pts[i] = Pff(rng.nextFloat() * (max_bounds - min_bounds) + min_bounds,
                     rng.nextFloat() * (max_bounds - min_bounds) + min_bounds);

    }
}

float shortest_distance(std::vector<Pff>& pts, float x, float y)
{
    float dist = 1000000000000.f;

    for (int i = 0; i < pts.size(); ++i)
    {
        float one = x - pts[i].first;
        float two = y - pts[i].second;

        float tmp = sqrt(one * one + two * two);

        if (tmp < dist) dist = tmp;
    }

    return dist;
}

// this is basically dart sampling with using rejection sampling
void initialize_non_colliding_points(std::vector<Pff>& pts,
                                     int num_tile_spots,
                                     float min_bounds,
                                     float max_bounds,
                                     pcg32& rng)
{
    pts = std::vector<Pff>(num_tile_spots);

    for (int i = 0; i < num_tile_spots; ++i)
    {
        // std::cout << "generating point: " << i << " out of: " << num_tile_spots << std::endl;
        int attempts = 0;
        bool not_found = true;

        while (attempts < 100 && not_found)
        {
            float x = rng.nextFloat() * (max_bounds - min_bounds) + min_bounds;
            float y = rng.nextFloat() * (max_bounds - min_bounds) + min_bounds;

            float dist = shortest_distance(pts, x, y);

            if (dist >= 35.f)
            {
                pts[i] = Pff(x, y);
                not_found = false;
            }
            else
            {
                attempts++;
            }
        }

        if (not_found)
        {
            pts[i] = Pff(1000000000.f * (i+1) * 30.f, 1000000000.f * (i+1) * 30.f);
        }
    }
}

void initialize_regular_colors(std::vector<Pix>& colors,
                               int num_colors,
                               Pix color_one,
                               Pix color_two)
{
    colors = std::vector<Pix>(num_colors);

    if (num_colors == 1)
    {
        colors[0].r = (color_one.r + color_two.r) / 2.0;
        colors[0].g = (color_one.g + color_two.g) / 2.0;
        colors[0].b = (color_one.b + color_two.b) / 2.0;
    }
    else
    {
        for (int i = 0; i < num_colors; ++i)
        {
            float scale = float(i) / float(num_colors - 1);

            colors[i].r = scale * color_one.r + (1.f - scale) * color_two.r;
            colors[i].g = scale * color_one.g + (1.f - scale) * color_two.g;
            colors[i].b = scale * color_one.b + (1.f - scale) * color_two.b;
        }
    }
}

void update_euc_positions(std::vector<Pff>& pts,
                          const std::vector<Pff>& vels)
{
    for (int i = 0; i < pts.size(); ++i)
    {
        // std::cout << "first before: " << pts[i].first << std::endl;
        // std::cout << "change: " << vels[i].first << std::endl;
        pts[i].first += vels[i].first;
        // std::cout << "first after: " << pts[i].first << std::endl;
        pts[i].second += vels[i].second;
    }
}

void update_euc_velocities(const std::vector<Pff>& pts,
                           std::vector<Pff>& vels,
                           const std::vector<Pff>& gravs,
                           float min_bounds,
                           float max_bounds)
{
    for (int i = 0; i < pts.size(); ++i)
    {
        Pff force = Pff(0.f, 0.f);

        for (int j = 0; j < gravs.size(); ++j)
        {
            float one = gravs[j].first - pts[i].first;
            float two = gravs[j].second - pts[i].second;

            float dist = sqrt(one * one + two * two);

            // std::cout << one / dist << std::endl;
            // float dist = one + two;

            force.first += one / dist * 2.0 / dist;
            force.second += two / dist * 2.0 / dist;

            // force.first += one / dist * 64.0 / dist;
            // force.second += two / dist * 64.0 / dist;
        }

        for (int j = 0; j < pts.size(); ++j)
        {
            if (j != i)
            {
                float one = pts[j].first - pts[i].first;
                float two = pts[j].second - pts[i].second;

                float dist = sqrt(one * one + two * two);

                if (dist <= 30.f && dist >= 5.f)
                {
                    force.first += one / dist * 30.0 / dist;
                    force.second += two / dist * 30.0 / dist;
                }
            }
        }

        vels[i].first += force.first;
        // std::cout << "Vel: " << vels[i].first << std::endl;
        vels[i].second += force.second;

        if (pts[i].first < min_bounds && vels[i].first < 0.f)
        {
            vels[i].first = -vels[i].first;
        }
        if (pts[i].first > max_bounds && vels[i].first > 0.f)
        {
            vels[i].first = -vels[i].first;
        }

        if (pts[i].second < min_bounds && vels[i].second < 0.f)
        {
            vels[i].second = -vels[i].second;
        }
        if (pts[i].second > max_bounds && vels[i].second > 0.f)
        {
            vels[i].second = -vels[i].second;
        }

        float mag = vels[i].first * vels[i].first +
                    vels[i].second * vels[i].second;

        if (mag > 100.f)
        {
            mag = sqrt(mag);
            vels[i].first = vels[i].first / mag * 10.f;
            vels[i].second = vels[i].second / mag * 10.f;
        }
    }
}

void draw_points(imedit::Image& image,
                 const std::vector<Pff>& pts,
                 int res)
{
    for (int i = 0; i < pts.size(); ++i)
    {
        int x = int(pts[i].first);
        int y = int(pts[i].second);

        for (int x_tmp = x - 4; x_tmp <= x + 4; ++x_tmp)
        {
            for (int y_tmp = y - 4; y_tmp <= y + 4; ++y_tmp)
            {
                if (x_tmp >= 0 && x_tmp < res && y_tmp >= 0 && y_tmp < res)
                {
                    image(x_tmp, y_tmp, 0) = 0.f;
                    image(x_tmp, y_tmp, 1) = 0.f;
                    image(x_tmp, y_tmp, 2) = 1.f;
                }
            }
        }

        // if (x >= 0 && x < res && y >= 0 && y < res)
        // {
        //     image(x, y, 0) = 0.f;
        //     image(x, y, 1) = 0.f;
        //     image(x, y, 2) = 1.f;
        // }
    }
}

int main(int argc, char* argv[])
{
    pcg32 rng = pcg32(0x4235, 0xac42);
    int res = 1024;
    int num_tile_spots = 256;
    int num_grav_spots = 20;
    int frames = 1000;
    float min_bounds = -2.0 * res;
    float max_bounds = 3.0 * res;
    std::string path_to_results = "results/";

    std::vector<Pff> euc_pts;
    std::vector<Pff> euc_vel;
    std::vector<Pff> grav_spots;
    std::vector<Pix> euc_colors;

    // initialize_random_points(euc_pts,
    //                          num_tile_spots,
    //                          min_bounds,
    //                          max_bounds,
    //                          rng);

    initialize_non_colliding_points(euc_pts,
                                    num_tile_spots,
                                    min_bounds,
                                    max_bounds,
                                    rng);

    initialize_random_points(grav_spots,
                             num_grav_spots,
                             min_bounds,
                             max_bounds,
                             rng);

    initialize_regular_colors(euc_colors,
                              num_tile_spots,
                              Pix(0.0, 0.0, 0.0),
                              Pix(1.0, 1.0, 1.0));

    euc_vel = std::vector<Pff>(num_tile_spots);

    for (int i = 0; i < frames; ++i)
    {
        std::cout << "rendering frame: " << i << " out of: " << frames << std::endl;
        imedit::Image image = imedit::Image(res, res, 3);

        // update the positions of the points
        update_euc_positions(euc_pts, euc_vel);

        // update the velocities of the points
        update_euc_velocities(euc_pts, euc_vel, grav_spots, min_bounds, max_bounds);

        // create the current image frame
        // manhattan_tiled_image(image, euc_pts, euc_colors);
        euclidean_tiled_image(image, euc_pts, euc_colors);

        // draw the points for debugging
        // draw_points(image, euc_pts, res);

        char str[4];
        snprintf(str, 4, "%03d", i);


        imedit::Image edge_image = imedit::edge_image(image);
        edge_image *= 4.0;

        image.write(path_to_results + "frame_" + std::string(str) + ".png");
        edge_image.write(path_to_results + "edge_" + std::string(str) + ".png");
    }
}
