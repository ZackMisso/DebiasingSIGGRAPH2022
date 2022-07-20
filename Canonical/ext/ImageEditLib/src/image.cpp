#include <imedit/image.h>
#include <stdexcept>
#include <algorithm>
#include <fstream>

#define STB_IMAGE_STATIC
#define STB_IMAGE_WRITE_STATIC

#define STB_IMAGE_IMPLEMENTATION
#include <stb/stb_image.h>

#define STB_IMAGE_WRITE_IMPLEMENTATION
#include <stb/stb_image_write.h>

#define TINYEXR_IMPLEMENTATION
#include <tinyexr/tinyexr.h>

namespace imedit
{

    RGBImage::RGBImage() : w(0), h(0)
    {
        pixels = std::vector<Pixel>();
    }

    RGBImage::RGBImage(int w, int h) : w(w), h(h)
    {
        pixels = std::vector<Pixel>(w * h);
    }

    RGBImage::RGBImage(const std::string &filename)
    {
        pixels = std::vector<Pixel>();
        w = 0;
        h = 0;

        read(filename);
    }

    void RGBImage::clear()
    {
        int siz = w * h;
        for (int i = 0; i < siz; ++i)
        {
            pixels[i] = Pixel(0.f);
        }
    }

    Float RGBImage::average() const
    {
        int siz = w * h;

        Float avg = 0.f;

        for (int i = 0; i < siz; ++i)
        {
            avg += pixels[i].sum();
        }

        avg /= Float(siz * 3);

        return avg;
    }

    Pixel RGBImage::average_pixel() const
    {
        Pixel avg = Pixel();
        int siz = h * w;

        for (int i = 0; i < siz; ++i)
        {
            avg += pixels[i];
        }

        return avg / Float(siz);
    }

    Float RGBImage::max() const
    {
        Float val = pixels[0].r;
        int siz = h * w;

        for (int i = 0; i < siz; ++i)
        {
            val = std::max(val, pixels[i].max());
        }

        return val;
    }

    Float RGBImage::min() const
    {
        Float val = pixels[0].r;
        int siz = h * w;

        for (int i = 0; i < siz; ++i)
        {
            val = std::min(val, pixels[i].min());
        }

        return val;
    }

    Pixel RGBImage::max_channel() const
    {
        Pixel max = pixels[0];
        int siz = h * w;

        for (int i = 1; i < siz; ++i)
        {
            max = Pixel::max(max, pixels[i]);
        }

        return max;
    }

    Pixel RGBImage::min_channel() const
    {
        Pixel min = pixels[0];
        int siz = h * w;

        for (int i = 1; i < siz; ++i)
        {
            min = Pixel::min(min, pixels[i]);
        }

        return min;
    }

    void RGBImage::brighten(Float factor)
    {
        (operator*=)(factor);
    }

    void RGBImage::contrast(Float factor, Float midpoint)
    {
        (operator-=)(midpoint);
        (operator*=)(factor);
        (operator+=)(midpoint);
    }

    void RGBImage::exposure(Float factor)
    {
        alterGamma(Float(1.0 / 2.2), Float(1.0));
        brighten(factor);
        alterGamma(Float(1.0), Float(1.0 / 2.2));
    }

    void RGBImage::alterGamma(Float oldGamma, Float newGamma)
    {
        Float power = newGamma / oldGamma;
        int siz = h * w;

        for (int i = 0; i < siz; ++i)
        {
            pixels[i].r = pow(pixels[i].r, power);
            pixels[i].g = pow(pixels[i].g, power);
            pixels[i].b = pow(pixels[i].b, power);
        }
    }

    bool RGBImage::read(const std::string &filename)
    {
        int wid;
        int hei;
        int dep;

        // std::cout << "READING" << std::endl;

        try
        {
            if (getExtension(filename) == "txt")
            {
                std::string line;
                std::ifstream file(filename);

                // std::cout << "HELLO" << std::endl;

                std::getline(file, line);
                int wid = std::stoi(line);
                std::getline(file, line);
                int hei = std::stoi(line);

                resize(wid, hei);

                for (int y = 0; y < h; ++y)
                {
                    for (int x = 0; x < w; ++x)
                    {
                        for (int z = 0; z < 3; ++z)
                        {
                            std::getline(file, line);
                            operator()(x, y, z) = (Float)std::stod(line);
                        }
                    }
                }

                return true;
            }
            else if (getExtension(filename) == "exr")
            {
                float *out;
                int tmp_w;
                int tmp_h;
                const char *err = nullptr;

                // std::cout << "bill" << std::endl;

                int ret = LoadEXR(&out, &tmp_w, &tmp_h, filename.c_str(), &err);
                // std::cout << "billy" << std::endl;
                if (ret != TINYEXR_SUCCESS)
                {
                    if (err)
                    {
                        std::cout << err << std::endl;
                        FreeEXRErrorMessage(err);
                        // std::cout << "bob" << std::endl;
                        // throw std::runtime_error("Could not load EXR image");
                    }
                }
                else
                {
                    resize(tmp_w, tmp_h);

                    for (int y = 0; y < h; ++y)
                    {
                        for (int x = 0; x < w; ++x)
                        {
                            for (int z = 0; z < 3; ++z)
                            {
                                operator()(x, y, z) = (Float)out[4 * (x + y * w) + z];
                                // if (operator()(x, y, z) < 0.0) std::cout << "SUCCESS" << std::endl;
                                // std::cout << "failure" << std::endl;
                            }
                        }
                    }

                    free(out);
                    return true;
                }
            }
            else if (stbi_is_hdr(filename.c_str()))
            {
                float *pxls = stbi_loadf(filename.c_str(),
                                         &wid,
                                         &hei,
                                         &dep,
                                         3);
                if (pxls)
                {
                    resize(wid, hei);

                    for (int y = 0; y < h; ++y)
                    {
                        for (int x = 0; x < w; ++x)
                        {
                            for (int z = 0; z < 3; ++z)
                            {
                                operator()(x, y, z) = (Float)pxls[3 * (x + y * w) + z];
                            }
                        }
                    }

                    stbi_image_free(pxls);
                    return true;
                }
                else
                {
                    throw std::runtime_error("Could not load HDR image");
                }
            }
            else
            {
                unsigned char *pxls = stbi_load(filename.c_str(),
                                                &wid,
                                                &hei,
                                                &dep,
                                                3);

                if (pxls)
                {
                    resize(wid, hei);

                    for (int y = 0; y < h; ++y)
                    {
                        for (int x = 0; x < w; ++x)
                        {
                            for (int z = 0; z < 3; ++z)
                            {
                                byteToVal(pxls[3 * (x + y * w) + z], operator()(x, y, z));
                            }
                        }
                    }

                    stbi_image_free(pxls);
                    return true;
                }
                else
                {
                    throw std::runtime_error("Could not load LDR image");
                }
            }
        }
        catch (const std::exception &e)
        {
            std::cerr << "Error reading Image for file: \"" << filename << "\":\n\t"
                      << stbi_failure_reason() << std::endl;
            return false;
        }

        return false;
    }

    bool RGBImage::write(const std::string &filename)
    {
        if (getExtension(filename) == "txt")
        {
            std::ofstream file(filename);
            file << width() << "\n";
            file << height() << "\n";

            for (int z = 0; z < 3; ++z)
            {
                for (int x = 0; x < w; ++x)
                {
                    for (int y = 0; y < h; ++y)
                    {
                        file << operator()(x, y, z) << "\n";
                    }
                }
            }

            file.close();
        }
        else
        {
            std::string extension = getExtension(filename);

            std::transform(extension.begin(),
                           extension.end(),
                           extension.begin(),
                           ::tolower);

            try
            {
                if (extension == "hdr")
                {
                    std::vector<float> pxls(h * w * 3, 1.f);
                    for (int x = 0; x < w; ++x)
                    {
                        for (int y = 0; y < h; ++y)
                        {
                            for (int z = 0; z < 3; ++z)
                            {
                                pxls[z + 3 * (x + y * w)] = (float)operator()(x, y, z);
                            }
                        }
                    }

                    if (!stb::stbi_write_hdr(filename.c_str(), w, h, 3, &pxls[0]))
                    {
                        throw std::runtime_error("Could not write HDR image");
                    }
                }
                else if (extension == "exr")
                {
                    EXRHeader header;
                    InitEXRHeader(&header);

                    EXRImage image;
                    InitEXRImage(&image);

                    image.num_channels = 3;

                    std::vector<float> images[3];
                    images[0].resize(w * h);
                    images[1].resize(w * h);
                    images[2].resize(w * h);

                    for (int i = 0; i < h; i++)
                    {
                        for (int j = 0; j < w; ++j)
                        {
                            images[0][i * w + j] = pixels[i * w + j].r;
                            images[1][i * w + j] = pixels[i * w + j].g;
                            images[2][i * w + j] = pixels[i * w + j].b;
                        }
                    }

                    float *image_ptr[3];
                    image_ptr[0] = &(images[2].at(0)); // B
                    image_ptr[1] = &(images[1].at(0)); // G
                    image_ptr[2] = &(images[0].at(0)); // R

                    image.images = (unsigned char **)image_ptr;
                    image.width = w;
                    image.height = h;

                    header.num_channels = 3;
                    header.channels = (EXRChannelInfo *)malloc(sizeof(EXRChannelInfo) * header.num_channels);
                    // Must be BGR(A) order, since most of EXR viewers expect this channel order.
                    strncpy(header.channels[0].name, "B", 255);
                    header.channels[0].name[strlen("B")] = '\0';
                    strncpy(header.channels[1].name, "G", 255);
                    header.channels[1].name[strlen("G")] = '\0';
                    strncpy(header.channels[2].name, "R", 255);
                    header.channels[2].name[strlen("R")] = '\0';

                    header.pixel_types = (int *)malloc(sizeof(int) * header.num_channels);
                    header.requested_pixel_types = (int *)malloc(sizeof(int) * header.num_channels);

                    for (int i = 0; i < header.num_channels; i++)
                    {
                        header.pixel_types[i] = TINYEXR_PIXELTYPE_FLOAT;          // pixel type of input image
                        header.requested_pixel_types[i] = TINYEXR_PIXELTYPE_HALF; // pixel type of output image to be stored in .EXR
                    }

                    const char *err;
                    int ret = SaveEXRImageToFile(&image, &header, filename.c_str(), &err);

                    if (ret != TINYEXR_SUCCESS)
                    {
                        fprintf(stderr, "Save EXR err: %s\n", err);
                        return ret;
                    }

                    // std::cout << "succeeded: " << filename << std::endl;

                    free(header.channels);
                    free(header.pixel_types);
                    free(header.requested_pixel_types);

                    return true;
                }
                else if (extension == "png" ||
                         extension == "bmp" ||
                         extension == "tga" ||
                         extension == "jpg" ||
                         extension == "jpeg")
                {
                    int outC = 4;
                    std::vector<unsigned char> pxls(h * w * outC, 255);

                    for (int x = 0; x < w; ++x)
                    {
                        for (int y = 0; y < h; ++y)
                        {
                            int z;
                            for (z = 0; z < 3; ++z)
                            {
                                pxls[z + outC * (x + y * w)] = valToByte(pow(operator()(x, y, z), 1.0 / 2.2));
                            }
                            for (; z < 3; ++z)
                            {
                                pxls[z + outC * (x + y * w)] = valToByte(pow(operator()(x, y, 0), 1.0 / 2.2));
                            }
                        }
                    }

                    if (extension == "png")
                    {
                        if (!stb::stbi_write_png(filename.c_str(), w, h,
                                                 outC, &pxls[0],
                                                 sizeof(unsigned char) * w * outC))
                        {
                            throw std::runtime_error("Could not write PNG image.");
                        }
                    }
                    else if (extension == "bmp")
                    {
                        if (!stb::stbi_write_bmp(filename.c_str(), w, h,
                                                 outC, &pxls[0]))
                        {
                            throw std::runtime_error("Could not write BMP image.");
                        }
                    }
                    else if (extension == "tga")
                    {
                        if (!stb::stbi_write_tga(filename.c_str(), w, h,
                                                 outC, &pxls[0]))
                        {
                            throw std::runtime_error("Could not write TGA image.");
                        }
                    }
                    else if (extension == "jpg" || extension == "jpeg")
                    {
                        if (!stb::stbi_write_jpg(filename.c_str(), w, h,
                                                 outC, &pxls[0], 100))
                        {
                            throw std::runtime_error("Could not write JPEG image.");
                        }
                    }
                    else
                    {
                        throw std::invalid_argument("Invalid extension");
                    }
                }
            }
            catch (const std::exception &e)
            {
                std::cout << "ERROR :: Image::write(" << filename << ") :: " << e.what() << std::endl;
            }
        }

        return true;
    }

    // bool RGBImage::writeChannel(const std::string& filename, int ch)
    // {
    //     return getChannel(ch).write(filename);
    // }

    float RGBImage::safeAccess(int j, int i, int k)
    {
        if (j < 0)
            j = 0;
        if (j >= w)
            j = w - 1;
        if (i < 0)
            i = 0;
        if (i >= h)
            i = h;
        if (k < 0)
            k = 0;
        if (k >= 3)
            k = 2;

        return operator()(j, i, k);
    }

    void RGBImage::resize(int cols, int rows)
    {
        pixels.resize(cols * rows);
        // TODO - not completely correct
        w = cols;
        h = rows;
    }

    void RGBImage::setZero()
    {
        for (int i = 0; i < pixels.size(); ++i)
        {
            pixels[i] = Pixel();
        }
    }

    // Image RGBImage::getChannel(int ch)
    // {
    //     Image image = Image(w, h);
    //
    //     for (int i = 0; i < h; ++i)
    //     {
    //         for (int j = 0; j < w; ++j)
    //         {
    //             image(j, h) = operator()(j, h);
    //         }
    //     }
    //
    //     return image;
    // }

    void RGBImage::setPixels(Float r, Float g, Float b)
    {
        for (int i = 0; i < h; ++i)
        {
            for (int j = 0; j < w; ++j)
            {
                operator()(j, i, 0) = r;
                operator()(j, i, 1) = g;
                operator()(j, i, 2) = b;
            }
        }
    }

    void RGBImage::setPixels(Pixel p)
    {
        setPixels(p.r, p.g, p.b);
    }

    void RGBImage::setPixel(int j, int i, Pixel p)
    {
        operator()(j, i, 0) = p.r;
        operator()(j, i, 1) = p.g;
        operator()(j, i, 2) = p.b;
    }

    void RGBImage::operator~()
    {
        int size = w * h * 3;

        for (int i = 0; i < size; ++i)
        {
            operator[](i) = 1.f - operator[](i);
        }
    }

    RGBImage RGBImage::operator+(const RGBImage &other) const
    {
        RGBImage image = RGBImage(w, h);

        int size = w * h * 3;

        for (int i = 0; i < size; ++i)
        {
            image[i] = operator[](i) + other[i];
        }

        return image;
    }

    RGBImage RGBImage::operator-(const RGBImage &other) const
    {
        RGBImage image = RGBImage(w, h);

        int size = w * h * 3;

        for (int i = 0; i < size; ++i)
        {
            image[i] = operator[](i) - other[i];
        }

        return image;
    }

    // this is component-wise multiplication
    RGBImage RGBImage::operator*(const RGBImage &other) const
    {
        RGBImage image = RGBImage(w, h);

        int size = w * h * 3;

        for (int i = 0; i < size; ++i)
        {
            image[i] = operator[](i) * other[i];
        }

        return image;
    }

    // this is component-wise division
    RGBImage RGBImage::operator/(const RGBImage &other) const
    {
        RGBImage image = RGBImage(w, h);

        int size = w * h * 3;

        for (int i = 0; i < size; ++i)
        {
            image[i] = operator[](i) / other[i];
        }

        return image;
    }

    RGBImage RGBImage::operator+(Float val) const
    {
        RGBImage image = RGBImage(w, h);

        int size = w * h * 3;

        for (int i = 0; i < size; ++i)
        {
            image[i] = operator[](i) + val;
        }

        return image;
    }

    RGBImage RGBImage::operator-(Float val) const
    {
        RGBImage image = RGBImage(w, h);

        int size = w * h * 3;

        for (int i = 0; i < size; ++i)
        {
            image[i] = operator[](i) - val;
        }

        return image;
    }

    RGBImage RGBImage::operator*(Float val) const
    {
        RGBImage image = RGBImage(w, h);

        int size = w * h * 3;

        for (int i = 0; i < size; ++i)
        {
            image[i] = operator[](i) * val;
        }

        return image;
    }

    RGBImage RGBImage::operator/(Float val) const
    {
        RGBImage image = RGBImage(w, h);

        int size = w * h * 3;

        for (int i = 0; i < size; ++i)
        {
            image[i] = operator[](i) / val;
        }

        return image;
    }

    void RGBImage::operator+=(const RGBImage &other)
    {
        int size = w * h * 3;

        for (int i = 0; i < size; ++i)
        {
            operator[](i) += other[i];
        }
    }

    void RGBImage::operator-=(const RGBImage &other)
    {
        int size = w * h * 3;

        for (int i = 0; i < size; ++i)
        {
            operator[](i) -= other[i];
        }
    }

    // component wise multiplication
    void RGBImage::operator*=(const RGBImage &other)
    {
        int size = w * h * 3;

        for (int i = 0; i < size; ++i)
        {
            operator[](i) *= other[i];
        }
    }

    // component wise division
    void RGBImage::operator/=(const RGBImage &other)
    {
        int size = w * h * 3;

        for (int i = 0; i < size; ++i)
        {
            operator[](i) /= other[i];
        }
    }

    void RGBImage::operator+=(Float val)
    {
        int size = w * h * 3;

        for (int i = 0; i < size; ++i)
        {
            operator[](i) += val;
        }
    }

    void RGBImage::operator-=(Float val)
    {
        int size = w * h * 3;

        for (int i = 0; i < size; ++i)
        {
            operator[](i) -= val;
        }
    }

    // component wise multiplication
    void RGBImage::operator*=(Float val)
    {
        int size = w * h * 3;

        for (int i = 0; i < size; ++i)
        {
            operator[](i) *= val;
        }
    }

    // component wise division
    void RGBImage::operator/=(Float val)
    {
        int size = w * h * 3;

        for (int i = 0; i < size; ++i)
        {
            operator[](i) /= val;
        }
    }

    Float &RGBImage::operator[](int index)
    {
        int loc = index / 3;

        return pixels[loc].access(index);
    }

    Float RGBImage::operator[](int index) const
    {
        int loc = index / 3;

        return pixels[loc].const_access(index);
    }

    Float &RGBImage::operator()(int x, int y, int z)
    {
        int index = w * y + x;
        return pixels[index].access(z);
    }

    Float RGBImage::operator()(int x, int y, int z) const
    {
        int index = w * y + x;
        return pixels[index].const_access(z);
    }

    Float RGBImage::operator()(float x, float y, int z) const
    {
        // TODO
        return operator()(int(x), int(y), z);
    }

    Float &RGBImage::operator()(float x, float y, int z)
    {
        // TODO
        return operator()(int(x), int(y), z);
    }

    Pixel &RGBImage::operator()(int x, int y)
    {
        return pixels[y * w + x];
    }

    Pixel RGBImage::operator()(int x, int y) const
    {
        return pixels[y * w + x];
    }

    Pixel &RGBImage::operator()(float x, float y)
    {
        // TODO
        return pixels[0];
    }

    Pixel RGBImage::operator()(float x, float y) const
    {
        // TODO
        return Pixel();
    }

    Float &RGBImage::filter_index(int x, int y, int z)
    {
        if (x < 0)
            x = 0;
        if (x > w - 1)
            x = w - 1;
        if (y < 0)
            y = 0;
        if (y > h - 1)
            y = h - 1;
        if (z < 0)
            z = 0;
        if (z > 2)
            z = 2;

        return pixels[y * w + x].access(z);
    }

    Float RGBImage::filter_index(int x, int y, int z) const
    {
        if (x < 0)
            x = 0;
        if (x > w - 1)
            x = w - 1;
        if (y < 0)
            y = 0;
        if (y > h - 1)
            y = h - 1;
        if (z < 0)
            z = 0;
        if (z > 2)
            z = 2;

        return pixels[y * w + x].const_access(z);
    }

}
