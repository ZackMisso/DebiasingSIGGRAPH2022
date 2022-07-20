#pragma once

namespace imedit
{

typedef float Float;

struct Pixel
{
    Pixel() : r(0.f), g(0.f), b(0.f) { }
    Pixel(Float v) : r(v), g(v), b(v) { }
    Pixel(Float r, Float g, Float b) : r(r), g(g), b(b) { }

    void convert_from_rgb_to_xyz()
    {
        // TODO
    }

    void convert_from_xyz_to_rgb()
    {
        // TODO
    }

    void convert_from_rgb_to_grey()
    {
        // TODO
    }

    void convert_grey_to_rgb()
    {
        // TODO
    }

    // TODO: maybe extend my vector implementation instead of rewriting
    //       all of this?

    Float sum() const
    {
        return r + g + b;
    }

    Pixel operator+(const Pixel& other) const
    {
        return Pixel(r + other.r,
                     g + other.g,
                     b + other.b);
    }

    Pixel operator-(const Pixel& other) const
    {
        return Pixel(r - other.r,
                     g - other.g,
                     b - other.b);
    }

    Pixel operator*(const Pixel& other) const
    {
        return Pixel(r * other.r,
                     g * other.g,
                     b * other.b);
    }

    Pixel operator/(const Pixel& other) const
    {
        return Pixel(r / other.r,
                     g / other.g,
                     b / other.b);
    }

    Pixel operator+(Float val) const
    {
        return Pixel(r + val,
                     g + val,
                     b + val);
    }

    Pixel operator-(Float val) const
    {
        return Pixel(r - val,
                     g - val,
                     b - val);
    }

    Pixel operator*(Float val) const
    {
        return Pixel(r * val,
                     g * val,
                     b * val);
    }

    Pixel operator/(Float val) const
    {
        return Pixel(r / val,
                     g / val,
                     b / val);
    }

    void operator+=(const Pixel& other)
    {
        r += other.r;
        g += other.g;
        b += other.b;
    }

    void operator-=(const Pixel& other)
    {
        r -= other.r;
        g -= other.g;
        b -= other.b;
    }

    void operator*=(const Pixel& other)
    {
        r *= other.r;
        g *= other.g;
        b *= other.b;
    }

    void operator/=(const Pixel& other)
    {
        r /= other.r;
        g /= other.g;
        b /= other.b;
    }

    void operator+=(Float val)
    {
        r += val;
        g += val;
        b += val;
    }

    void operator-=(Float val)
    {
        r -= val;
        g -= val;
        b -= val;
    }

    void operator*=(Float val)
    {
        r *= val;
        g *= val;
        b *= val;
    }

    void operator/=(Float val)
    {
        r /= val;
        g /= val;
        b /= val;
    }

    bool operator!=(const Pixel& other)
    {
        return r != other.r &&
               g != other.g &&
               b != other.b;
    }

    Float min() const
    {
        return std::min(r, std::min(g, b));
    }

    Float max() const
    {
        return std::max(r, std::max(g, b));
    }

    Float& access(int index)
    {
        if (index % 3 == 0)
            return r;
        if (index % 3 == 1)
            return g;
        return b;
    }

    Float const_access(int index) const
    {
        if (index % 3 == 0)
            return r;
        if (index % 3 == 1)
            return g;
        return b;
    }

    static Pixel max(const Pixel& one, const Pixel& two)
    {
        Pixel val;

        val.r = std::max(one.r, two.r);
        val.g = std::max(one.g, two.g);
        val.b = std::max(one.b, two.b);

        return val;
    }

    static Pixel min(const Pixel& one, const Pixel& two)
    {
        Pixel val;

        val.r = std::min(one.r, two.r);
        val.g = std::min(one.g, two.g);
        val.b = std::min(one.b, two.b);

        return val;
    }

    Float r;
    Float g;
    Float b;
};

}
