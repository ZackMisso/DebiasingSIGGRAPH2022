#include <iostream>
#include "imedit/image.h"
#include "imedit/procedural.h"
#include "imedit/filter.h"
#include "imedit/im_util.h"
#include "pcg/pcg32.h"

using namespace std;
using namespace imedit;

void noise_one()
{
    int res = 4096;
    std::string extension = ".hdr";
    double turb_period = double(res) / 64.0;
    double noise_period = double(res) / 16.0;
    double sharp_weight = 0.3;
    int num_tiles = 45;

    std::cout << "creating turb image" << std::endl;
    Image turb_image = Image(res, res, 3);
    turbulence_image_xy(turb_image, turb_period);
    turb_image.write("turb"+extension);
    remap_range_lin(turb_image);

    std::cout << "creating noise image" << std::endl;
    Image noise_image = Image(res, res, 3);
    noise_image_xy(noise_image, noise_period);
    im_abs(noise_image);

    std::cout << "creating sharp image" << std::endl;
    Image sharp_image = Image(res, res, 3);
    sharpen3x3(turb_image, sharp_image);
    remap_avg(sharp_image, 0.0);
    sharp_image = sharp_image * sharp_weight + turb_image;
    sharp_image.write("sharp"+extension);

    std::cout << "creating green image" << std::endl;
    Image green_image = Image(res, res, 3);
    green_image.setPixels(0.03, 0.1, 0.5);
    green_image *= 2.12;

    std::cout << "finishing up noise image" << std::endl;
    noise_image = noise_image * green_image * 2.0;
    noise_image.write("noise"+extension);

    std::cout << "creating sharp green image" << std::endl;
    green_image.write("green"+extension);
    green_image = green_image * sharp_image;
    double max = green_image.max();
    Image max_image = Image(res, res, 3);
    max_image.setPixels(max, max, max);
    green_image = max_image - green_image;
    green_image.write("green_sharp"+extension);

    std::cout << "creating manhattan distance tile image" << std::endl;
    Image man_tile_image = Image(res, res, 3);
    std::vector<std::pair<int, int> > pts = std::vector<std::pair<int, int> >();
    pcg32 rng = pcg32(0x135fa, 0xdbc45);
    for (int i = 0; i < num_tiles; ++i)
    {
        int one = (int)(rng.nextDouble() * res);
        int two = (int)(rng.nextDouble() * res);

        pts.push_back(std::pair<int, int>(one, two));
    }
    manhattan_dist_from_points_image(man_tile_image, pts);
    man_tile_image /= man_tile_image.max();
    man_tile_image.write("man_tile"+extension);

    std::cout << "creating manhattan tiled image" << std::endl;
    Image tile_image = Image(res, res, 3);
    std::vector<Pixel> pixels = std::vector<Pixel>();
    for (int i = 0; i < num_tiles; ++i)
    {
        Pixel pixel;
        pixel.r = rng.nextDouble();
        pixel.g = rng.nextDouble();
        pixel.b = rng.nextDouble();

        pixels.push_back(pixel);
    }
    manhattan_tiled_image(tile_image, pts, pixels);
    tile_image.write("tiled"+extension);

    std::cout << "creating euclidean distance tile image" << std::endl;
    Image euc_dist_image = Image(res, res, 3);
    std::vector<std::pair<Float, Float> > euc_pts = std::vector<std::pair<Float, Float> >();
    // pcg32 rng = pcg32(0x135fa, 0xdbc45);
    for (int i = 0; i < num_tiles; ++i)
    {
        double one = (double)(rng.nextDouble() * double(res));
        double two = (double)(rng.nextDouble() * double(res));

        euc_pts.push_back(std::pair<Float, Float>(one, two));
    }
    euclidean_dist_from_points_image(euc_dist_image, euc_pts);
    euc_dist_image /= euc_dist_image.max();
    euc_dist_image.write("euc_dist"+extension);

    std::cout << "creating euclidean tiled image" << std::endl;
    Image euc_tile_image = Image(res, res, 3);
    pixels.clear();
    for (int i = 0; i < num_tiles; ++i)
    {
        Pixel pixel;
        pixel.r = rng.nextDouble();
        pixel.g = rng.nextDouble();
        pixel.b = rng.nextDouble();

        pixels.push_back(pixel);
    }
    euclidean_tiled_image(euc_tile_image, euc_pts, pixels);
    euc_tile_image.write("euc_tiled"+extension);
}

void create_tiled_image(const std::string& name,
                        const std::vector<Pixel>& colors,
                        unsigned long seed_one,
                        unsigned long seed_two,
                        int res,
                        bool is_manhatan)
{
    pcg32 rng = pcg32(seed_one, seed_two);

    Image image = Image(res, res, 3);

    if (is_manhatan)
    {
        std::vector<std::pair<int, int> > pts = std::vector<std::pair<int, int> >();
        for (int i = 0; i < colors.size(); ++i)
        {
            int one = (int)(rng.nextDouble() * res);
            int two = (int)(rng.nextDouble() * res);

            pts.push_back(std::pair<int, int>(one, two));
        }
        manhattan_tiled_image(image, pts, colors);
    }
    else
    {
        std::vector<std::pair<Float, Float> > euc_pts = std::vector<std::pair<Float, Float> >();
        for (int i = 0; i < colors.size(); ++i)
        {
            double one = (double)(rng.nextDouble() * double(res));
            double two = (double)(rng.nextDouble() * double(res));

            euc_pts.push_back(std::pair<Float, Float>(one, two));
        }
        euclidean_tiled_image(image, euc_pts, colors);
    }

    image *= 2.0;

    image.write(name);
}

void create_pokedot_image(std::string name,
                          Pixel base,
                          double min_dist,
                          double max_dist,
                          unsigned long seed_one,
                          unsigned long seed_two,
                          int dots,
                          int res,
                          bool is_manhatan)
{
    pcg32 rng = pcg32(seed_one, seed_two);

    Image image = Image(res, res, 3);
    image.setPixels(0.0, 0.0, 0.0);

    if (is_manhatan)
    {
        for (int i = 0; i < dots; ++i)
        {
            int dist = int(rng.nextDouble() * (max_dist - min_dist) + min_dist);
            int xpos = int(rng.nextDouble() * double(res));
            int ypos = int(rng.nextDouble() * double(res));

            splat_manhattan(image, base, xpos, ypos, dist);
        }
    }
    else
    {
        for (int i = 0; i < dots; ++i)
        {
            double dist = rng.nextDouble() * (max_dist - min_dist) + min_dist;
            double xpos = rng.nextDouble() * double(res);
            double ypos = rng.nextDouble() * double(res);

            splat_euclidean(image, base, xpos, ypos, dist);
        }
    }

    image /= image.max();
    // image.setDark(1.0);

    Image one_image = Image(res, res, 3);
    one_image.setPixels(1.0, 1.0, 1.0);
    image = one_image - image;
    image *= 2.0;

    image.write(name);
}

void create_white_image(const std::string& name,
                        const std::vector<Pixel>& colors,
                        unsigned long seed_one,
                        unsigned long seed_two,
                        int res,
                        bool is_manhatan)
{
    pcg32 rng = pcg32(seed_one, seed_two);

    Image image = Image(res, res, 3);

    for (int i = 0; i < image.size(); ++i)
    {
        image[i] = 0.9;
    }

    image.write(name);
}

void create_black_image(const std::string& name,
                        const std::vector<Pixel>& colors,
                        unsigned long seed_one,
                        unsigned long seed_two,
                        int res,
                        bool is_manhatan)
{
    pcg32 rng = pcg32(seed_one, seed_two);

    Image image = Image(res, res, 3);

    for (int i = 0; i < image.height(); ++i)
    {
        for (int j = 0; j < image.width(); ++j)
        {
            image(j, i, 0) = 0.04;
            image(j, i, 1) = 0.04;
            image(j, i, 2) = 0.04;
        }
    }

    image.write(name);
}

void generate_pawn_scene_board(const std::vector<Pixel>& pixels,
                               Pixel dot_base,
                               const std::string base_name,
                               unsigned long seed_one,
                               unsigned long seed_two,
                               double min_dot_dist,
                               double max_dot_dist,
                               int res,
                               int dots,
                               bool manhattan)
{
    pcg32 global_rng = pcg32(seed_one, seed_two);

    for (int i = 0; i < 11; ++i)
    {
        for (int j = 0; j < 11; ++j)
        {
            std::cout << "creating texture for tile: (" << j << "," << i << ")" << std::endl;

            std::string name = base_name + "_" + to_string(i) + "_" + to_string(j) + ".hdr";

            if ((i+j)%2 == 0)
            {
                // // create tiled image
                // create_tiled_image(name,
                //                    pixels,
                //                    global_rng.nextUInt(),
                //                    global_rng.nextUInt(),
                //                    res,
                //                    manhattan);

                create_black_image(name,
                                   pixels,
                                   global_rng.nextUInt(),
                                   global_rng.nextUInt(),
                                   res,
                                   manhattan);
            }
            else
            {
                // // create dotted image
                // create_pokedot_image(name,
                //                      dot_base,
                //                      min_dot_dist,
                //                      max_dot_dist,
                //                      global_rng.nextUInt(),
                //                      global_rng.nextUInt(),
                //                      dots,
                //                      res,
                //                      manhattan);

                create_white_image(name,
                                   pixels,
                                   global_rng.nextUInt(),
                                   global_rng.nextUInt(),
                                   res,
                                   manhattan);
            }
        }
    }
}

std::vector<Pixel> create_pixels(double min_h, double max_h,
                                 double min_s, double max_s,
                                 double min_l, double max_l,
                                 unsigned seed_one, unsigned seed_two,
                                 int num_pixels)
{
    std::vector<Pixel> pixels = std::vector<Pixel>();
    pcg32 rng = pcg32(seed_one, seed_two);

    for (int i = 0; i < num_pixels; ++i)
    {
        Pixel pixel;
        pixel.r = rng.nextDouble() * (max_h - min_h) + min_h;
        pixel.g = rng.nextDouble() * (max_s - min_s) + min_s;
        pixel.b = rng.nextDouble() * (max_l - min_l) + min_l;

        hsl_to_rgb(pixel);

        pixels.push_back(pixel);
    }

    return pixels;
}

void generate_mesh_image(std::string name,
                         double min_period,
                         double max_period,
                         unsigned long seed_one,
                         unsigned long seed_two,
                         int num_meshes,
                         int res,
                         double min_h,
                         double max_h,
                         double min_s,
                         double max_s,
                         double min_l,
                         double max_l)
{
    pcg32 rng = pcg32(seed_one, seed_two);

    for (int i = 0; i < num_meshes; ++i)
    {
        std::cout << "mesh: " << i << std::endl;

        double noise_period = (max_period - min_period) + min_period;

        Pixel pixel;
        pixel.r = rng.nextDouble() * (max_h - min_h) + min_h;
        pixel.g = rng.nextDouble() * (max_s - min_s) + min_s;
        pixel.b = rng.nextDouble() * (max_l - min_l) + min_l;

        std::cout << "hue: " << pixel.r << std::endl;
        std::cout << "sat: " << pixel.g << std::endl;
        std::cout << std::endl;

        hsl_to_rgb(pixel);

        Image noise_image = Image(res, res, 3);
        Image color_image = Image(res, res, 3);

        color_image.setPixels(pixel);

        noise_image_xy(noise_image, noise_period, (double)rng.nextUInt(), (double)rng.nextUInt());
        im_abs(noise_image);
        noise_image = noise_image * color_image * 4.0;
        noise_image.write(name + "_" + std::to_string(i) + ".hdr");
    }
}

int main()
{
    // noise_one();

    pcg32 rng = pcg32(0xfa23a, 0x980ac);
    Pixel base_dot;
    // double min_h = 55.0 / 360.0;
    // double max_h = 105.0 / 360.0;
    // double min_s = 0.35;
    // double max_s = 0.75;
    // double min_l = 0.32;
    // double max_l = 0.53;
    double min_h = 330.0 / 360.0;
    double max_h = 360.0 / 360.0;
    double min_s = 0.20;
    double max_s = 0.60;
    double min_l = 0.30;
    double max_l = 0.56;
    int res_obj = 4096;
    int res_board = 512;
    int num_tiles = 45;
    int num_dots = 50;
    double min_dot_dist = double(res_board) / 10.0;
    double max_dot_dist = double(res_board) / 5.0;

    base_dot.r = 1.0 - 337.0 / 360.0;
    base_dot.g = 0.66;
    base_dot.b = 0.86;
    hsl_to_rgb(base_dot);

    std::vector<Pixel> colors = create_pixels(min_h, max_h,
                                              min_s, max_s,
                                              min_l, max_l,
                                              rng.nextUInt(), rng.nextUInt(),
                                              num_tiles);

    generate_pawn_scene_board(colors,
                              base_dot,
                              "/Users/corneria/Documents/Research/testscenes/chess/textures/generated_2/board",
                              rng.nextUInt(),
                              rng.nextUInt(),
                              min_dot_dist,
                              max_dot_dist,
                              res_board,
                              num_dots,
                              false);

    int mesh_res = 1024;
    double mesh_min_period = double(mesh_res / 40.0);
    double mesh_max_period = double(mesh_res / 16.0);
    double mesh_min_h = 100.0 / 360.0;
    double mesh_max_h = 140.0 / 360.0;
    double mesh_min_s = 0.62;
    double mesh_max_s = 0.84;
    double mesh_min_l = 0.60;
    double mesh_max_l = 0.90;
    int num_meshes = 16;

    generate_mesh_image("/Users/corneria/Documents/Research/testscenes/chess/textures/generated_2/mesh",
                        mesh_min_period,
                        mesh_max_period,
                        rng.nextUInt(),
                        rng.nextUInt(),
                        num_meshes,
                        mesh_res,
                        mesh_min_h,
                        mesh_max_h,
                        mesh_min_s,
                        mesh_max_s,
                        mesh_min_l,
                        mesh_max_l);

    return 0;
}
