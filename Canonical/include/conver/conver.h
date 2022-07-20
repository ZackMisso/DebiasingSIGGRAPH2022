#pragma once

#include <iostream>
#include <fstream>
#include <pcg32.h>
#include <vector>
#include <string>
#include <tgmath.h>
#include <assert.h>
#include <cstdint>
#include <cstring>

typedef double Float;

static Float ShadowEpsilon = 0.0001f;
static Float Pi = 3.14159265358979323846;
static Float InvPi = 0.31830988618379067154;
static Float Inv2Pi = 0.15915494309189533577;
static Float Inv4Pi = 0.07957747154594766788;
static Float PiOver2 = 1.57079632679489661923;
static Float PiOver4 = 0.78539816339744830961;
static Float Sqrt2 = 1.41421356237309504880;

static int HeatTrials = 1024 * 64;
static int HeatSamples = 1;
// static int HeatRes = 10;
static int HeatRes = 512;

static int PPMSteps = 1024 * 1024 * 16;
static int PPMBasePhotons = 10000;
static double PPMRadius = 0.125;

static Float MaxFloat = std::numeric_limits<Float>::max();
static Float Infinity = std::numeric_limits<Float>::infinity();
static Float MachineEpsilon =
    std::numeric_limits<Float>::epsilon() * 0.5;

// problem parameter definitions
static int PHOTON_EMITTER_POWER = 0;
static int PHOTON_EMITTER_Z = 1;

// taken from my renderer
inline void feign_sincos(Float theta, Float *_sin, Float *_cos)
{
    *_sin = sinf(theta);
    *_cos = cosf(theta);
}

// utility method
static bool is_power_of_two(int val)
{
    int i = 1;
    while (i != 0)
    {
        if (val == i)
            return true;
        i <<= 1;
    }
    return false;
}

// Global Inline Functions
inline uint32_t FloatToBits(float f)
{
    uint32_t ui;
    memcpy(&ui, &f, sizeof(float));
    return ui;
}

inline float BitsToFloat(uint32_t ui)
{
    float f;
    std::memcpy(&f, &ui, sizeof(uint32_t));
    return f;
}

inline uint64_t FloatToBits(double f)
{
    uint64_t ui;
    std::memcpy(&ui, &f, sizeof(double));
    return ui;
}

inline double BitsToFloat(uint64_t ui)
{
    double f;
    std::memcpy(&f, &ui, sizeof(uint64_t));
    return f;
}

inline float NextFloatUp(float v)
{
    // Handle infinity and negative zero for _NextFloatUp()_
    if (std::isinf(v) && v > 0.)
        return v;
    if (v == -0.f)
        v = 0.f;

    // Advance _v_ to next higher float
    uint32_t ui = FloatToBits(v);
    if (v >= 0)
        ++ui;
    else
        --ui;
    return BitsToFloat(ui);
}

inline float NextFloatDown(float v)
{
    // Handle infinity and positive zero for _NextFloatDown()_
    if (std::isinf(v) && v < 0.)
        return v;
    if (v == 0.f)
        v = -0.f;
    uint32_t ui = FloatToBits(v);
    if (v > 0)
        --ui;
    else
        ++ui;
    return BitsToFloat(ui);
}

inline double NextFloatUp(double v, int delta = 1)
{
    if (std::isinf(v) && v > 0.)
        return v;
    if (v == -0.f)
        v = 0.f;
    uint64_t ui = FloatToBits(v);
    if (v >= 0.)
        ui += delta;
    else
        ui -= delta;
    return BitsToFloat(ui);
}

inline double NextFloatDown(double v, int delta = 1)
{
    if (std::isinf(v) && v < 0.)
        return v;
    if (v == 0.f)
        v = -0.f;
    uint64_t ui = FloatToBits(v);
    if (v > 0.)
        ui -= delta;
    else
        ui += delta;
    return BitsToFloat(ui);
}

static inline Float mod(Float val, Float div)
{
    Float tmp = (val / div);
    return tmp - floor(tmp);
}

inline Float Radians(Float deg) { return (Pi / 180) * deg; }

class NotImplementedException : public std::runtime_error
{
public:
    NotImplementedException() : std::runtime_error("Exception: Method Not Implemented") {}

    NotImplementedException(std::string str) : std::runtime_error("Exception: Method Not Implemented: " + str)
    {
        std::cout << str << std::endl;
    }
};

class ShouldNotEvaluateException : public std::runtime_error
{
public:
    ShouldNotEvaluateException() : std::runtime_error("Exception: Method Not Implemented") {}

    ShouldNotEvaluateException(std::string str) : std::runtime_error("Exception: Method Not Implemented: " + str)
    {
        std::cout << str << std::endl;
    }
};
