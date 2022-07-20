#include <iostream>
#include "imedit/image.h"
#include "imedit/procedural.h"
#include "imedit/filter.h"
#include "imedit/im_util.h"
#include "imedit/im_color_maps.h"
#include "pcg/pcg32.h"

struct Complex2f
{
public:
    Complex2f() : real(0.0), imag(0.0) { }

    Complex2f(double real)
        : real(real), imag(0.0) { }

    Complex2f(double real, double imag)
        : real(real), imag(imag) { }

    Complex2f(const Complex2f& other)
        : real(other.real),
          imag(other.imag) { }

    double real;
    double imag;

    Complex2f operator+(double r) const
    {
        return Complex2f(real + r,
                         imag);
    }

    Complex2f operator-(double r) const
    {
        return Complex2f(real - r,
                         imag);
    }

    Complex2f operator+(const Complex2f& other) const
    {
        return Complex2f(real + other.real,
                         imag + other.imag);
    }

    Complex2f operator-(const Complex2f& other) const
    {
        return Complex2f(real - other.real,
                         imag - other.imag);
    }

    void operator+=(const Complex2f& other)
    {
        real += other.real;
        imag += other.imag;
    }

    void operator-=(const Complex2f& other)
    {
        real += other.real;
        imag += other.imag;
    }

    Complex2f operator*(const Complex2f& other) const
    {
        double r = real * other.real - imag * other.imag;
        double i = real * other.imag + imag * other.real;

        return Complex2f(r, i);
    }

    double norm() const
    {
        return real * real + imag * imag;
    }
};

int main(int argc, char* argv[])
{
    std::cout << "starting mandlebrot visualization" << std::endl;

    std::string publish_command = "ffmpeg -r 24 -f image2 -i mandlebrot_vis/mandlebrot_%04d.png -vcodec mpeg4 -vb 20M -minrate 20M -maxrate 30M mandlebrot_vis/mandlebrot.mp4";

    // int x_res = 64;
    // int y_res = 64;

    int x_res = 1920;
    int y_res = 1080;

    int frames = 1500;

    system("mkdir mandlebrot_vis");

    double max_real = 2.0;
    double min_real = -2.0;
    double max_imag = 2.0 * double(y_res) / double(x_res);
    double min_imag = -2.0 * double(y_res) / double(x_res);
    int max_iterations = 3000;
    double max_value = 10000.0;

    for (int k = 0; k < frames; ++k)
    // for (int k = 0; k < frames; k+=100)
    // for (int k = 700; k < 701; k+=100)
    {
        pcg32 rng(k * 0x1ac4, k + 0xca23);

        imedit::Image image = imedit::Image(x_res, y_res);
        std::cout << "creating frame: " << k << std::endl;

        // double max_values[3];
        // max_values[0] = 10.0;
        // max_values[1] = 10.0;
        // max_values[2] = 10.0;
        //
        // int max_iters[3];
        // max_iters[0] = 5000;
        // max_iters[1] = 500;
        // max_iters[2] = 50;

        int pixel_samples = 128;

        std::vector<imedit::Pixel> colors = std::vector<imedit::Pixel>();
        imedit::color_map_inferno(colors);

        for (int i = 0; i < y_res; ++i)
        {
            for (int j = 0; j < x_res; ++j)
            {
                // imedit::Pixel results = imedit::Pixel(0.f);
                std::vector<double> results = std::vector<double>();

                // for (int g = 0; g < 3; ++g)
                // {
                    for (int s = 0; s < pixel_samples; ++s)
                    {
                        double real = (max_real - min_real) * double(j) / double(x_res) + min_real;
                        double imag = (max_imag - min_imag) * double(i) / double(y_res) + min_imag;

                        Complex2f val = Complex2f(real, imag);

                        int m = 1;
                        Complex2f c = Complex2f();

                        if (k < 200)
                        {
                            double x_loc = (2.0 / double(x_res) * 2.0 * rng.nextDouble());
                            double y_loc = (2.0 / double(y_res) * 2.0 * rng.nextDouble());

                            c.real = -1.3 * double(k) / double(200) + x_loc;
                            c.imag += y_loc;
                        }
                        else if (k < 400)
                        {
                            double x_loc = (2.0 / double(x_res) * 2.0 * rng.nextDouble());
                            double y_loc = (2.0 / double(y_res) * 2.0 * rng.nextDouble());

                            double proxy = double(k - 200) / 199.0;

                            c.real = (1.0 - proxy) * -1.3 +
                                     proxy * real + x_loc;
                            c.imag = (1.0 - proxy) * 0.0 +
                                     proxy * imag + y_loc;
                        }
                        else if (k < 600)
                        {
                            double proxy = double(k - 400) / 199.0;

                            double x_loc = (2.0 / double(x_res) * 2.0 * rng.nextDouble());
                            double y_loc = (2.0 / double(y_res) * 2.0 * rng.nextDouble());

                            c.real = (1.0 - proxy) * real +
                                     proxy * -real + x_loc;
                            c.imag = (1.0 - proxy) * imag +
                                     proxy * -imag + y_loc;
                        }
                        else if (k < 700)
                        {
                            double centering_proxy = std::min(double(k - 600) / 100.0, 1.0);

                            double end_center_x = 0.514910199998999;
                            double end_center_y = 0.508080499999996;

                            double center_x = centering_proxy * end_center_x;
                            double center_y = centering_proxy * end_center_y;

                            double tmp_max_r = center_x + 2.0;
                            double tmp_min_r = center_x - 2.0;

                            double tmp_max_i = center_x + 2.0 * double(y_res) / double(x_res);
                            double tmp_min_i = center_x - 2.0 * double(y_res) / double(x_res);

                            double x_loc = (2.0 / double(x_res) * 2.0 * rng.nextDouble());
                            double y_loc = (2.0 / double(y_res) * 2.0 * rng.nextDouble());

                            real = (tmp_max_r - tmp_min_r) * double(j) / double(x_res) + tmp_min_r + x_loc;
                            imag = (tmp_max_i - tmp_min_i) * double(i) / double(y_res) + tmp_min_i + y_loc;

                            val.real = real;
                            val.imag = imag;

                            c.real = -real;
                            c.imag = -imag;
                        }
                        else if (k < 1500)
                        {
                            double end_center_x = 0.514910199998999;
                            double end_center_y = 0.508080499999996;

                            double end_scale = 0.000000000000005;

                            double proxy = std::min(double(k - 700) / 799.0, 1.0);

                            double scale_start = log(2.0);
                            double scale_end = log(end_scale);

                            double log_scale = proxy * scale_end + (1.0 - proxy) * scale_start;
                            double scale = exp(log_scale);

                            // std::cout << "scale: " << scale << std::endl;
                            // std::cout << "end_scale: " << end_scale << std::endl;
                            // std::cout << "proxy: " << proxy << std::endl;

                            // std::cout << "scale_start: " << scale_start << std::endl;
                            // std::cout << "scale_end: " << scale_end << std::endl;
                            // std::cout << "log_scale: " << log_scale << std::endl;
                            // std::cout << "scale: " << scale << std::endl;
                            // std::cout << std::endl;

                            // double centering_proxy = std::min(double(k - 600) / 72.0, 1.0);

                            double center_x = end_center_x;
                            double center_y = end_center_y;

                            // double scale = end_scale * proxy + (1.0 - proxy) * 2.0;

                            double tmp_max_r = center_x + scale;
                            double tmp_min_r = center_x - scale;

                            double x_loc = (scale / double(x_res) * 2.0 * rng.nextDouble());
                            double y_loc = (scale / double(y_res) * 2.0 * rng.nextDouble());

                            double tmp_max_i = center_y + scale * double(y_res) / double(x_res);
                            double tmp_min_i = center_y - scale * double(y_res) / double(x_res);

                            // std::cout << "scale: " << scale << std::endl;

                            real = (tmp_max_r - tmp_min_r) * double(j) / double(x_res) + tmp_min_r + x_loc;
                            imag = (tmp_max_i - tmp_min_i) * double(i) / double(y_res) + tmp_min_i + y_loc;

                            val.real = real;
                            val.imag = imag;

                            c.real = -real;
                            c.imag = -imag;
                        }

                        while (val.norm() < max_value && m < max_iterations)
                        {
                            m++;
                            val = val*val + c;
                        }

                        results.push_back(double(m) / double(max_iterations+1));

                        // image(j, i) = double(m) / double(max_iterations+1);

                        // if (g==0) results.r += double(m) / double(max_iters[g]+1);
                        // if (g==1) results.g += double(m) / double(max_iters[g]+1);
                        // if (g==2) results.b += double(m) / double(max_iters[g]+1);
                    }
                // }

                for (int t = 0; t < results.size(); ++t)
                {
                    double max_value_1 = -log(1.0 / double(max_iterations+1));
                    double min_value_1 = -log(double(max_iterations) / double(max_iterations+1));
                    double value_1 = (-log(results[t])-min_value_1) / (max_value_1-min_value_1);

                    double max_value_2 = log(-log(1.0 / double(max_iterations+1)));
                    double min_value_2 = log(-log(double(max_iterations) / double(max_iterations+1)));
                    double value_2 = (log(-log(results[t]))-min_value_2) / (max_value_2-min_value_2);

                    if (k < 900)
                        image(j, i) += imedit::color_from_proxy(colors, value_1);
                    else if (k > 1200)
                        image(j, i) += imedit::color_from_proxy(colors, value_2);
                    else
                    {
                        double value_proxy = double(k-900) / double(300.0);
                        image(j, i) += imedit::color_from_proxy(colors, value_2*value_proxy+
                                                                       value_1 * (1.0-value_proxy));
                    }
                }

                image(j, i) /= double(pixel_samples);
            }
        }

        // // convert to color map
        // for (int i = 0; i < res; ++i)
        // {
        //     for (int j = 0; j < res; ++j)
        //     {
        //         double max_value_1 = -log(1.0 / double(max_iterations+1));
        //         double min_value_1 = -log(double(max_iterations) / double(max_iterations+1));
        //         double value_1 = (-log(image(j, i).r)-min_value_1) / (max_value_1-min_value_1);
        //
        //         double max_value_2 = log(-log(1.0 / double(max_iterations+1)));
        //         double min_value_2 = log(-log(double(max_iterations) / double(max_iterations+1)));
        //         double value_2 = (log(-log(image(j, i).r))-min_value_2) / (max_value_2-min_value_2);
        //
        //         if (k < 800)
        //             image(j, i) = imedit::color_from_proxy(colors, value_1);
        //         else if (k > 1100)
        //             image(j, i) = imedit::color_from_proxy(colors, value_2);
        //         else
        //         {
        //             double value_proxy = double(k-800) / double(300.0);
        //             image(j, i) = imedit::color_from_proxy(colors, value_2*value_proxy+
        //                                                            value_1 * (1.0-value_proxy));
        //         }
        //     }
        // }



        // convert to color map
        // for (int i = 0; i < y_res; ++i)
        // {
        //     for (int j = 0; j < x_res; ++j)
        //     {
        //         double max_value_1 = -log(1.0 / double(max_iterations+1));
        //         double min_value_1 = -log(double(max_iterations) / double(max_iterations+1));
        //         double value_1 = (-log(image(j, i).r)-min_value_1) / (max_value_1-min_value_1);
        //
        //         double max_value_2 = log(-log(1.0 / double(max_iterations+1)));
        //         double min_value_2 = log(-log(double(max_iterations) / double(max_iterations+1)));
        //         double value_2 = (log(-log(image(j, i).r))-min_value_2) / (max_value_2-min_value_2);
        //
        //         if (k < 800)
        //             image(j, i) = imedit::color_from_proxy(colors, value_1);
        //         else if (k > 1100)
        //             image(j, i) = imedit::color_from_proxy(colors, value_2);
        //         else
        //         {
        //             double value_proxy = double(k-800) / double(300.0);
        //             image(j, i) = imedit::color_from_proxy(colors, value_2*value_proxy+
        //                                                            value_1 * (1.0-value_proxy));
        //         }
        //     }
        // }

        char str[5];
        snprintf(str, 5, "%04d", k);
        std::string name = "mandlebrot_vis/mandlebrot_" + std::string(str);

        image.write(name + ".exr");
        image.write(name + ".png");
    }

    system(publish_command.c_str());

    return 0;
}
