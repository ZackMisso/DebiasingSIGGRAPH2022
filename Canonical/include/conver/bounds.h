#pragma once

#include <conver/conver.h>

// this is an abstraction for sample bounds

template <typename T>
struct Bounds
{
public:
    virtual T uniformSample(pcg32 &rng) = 0;
    virtual T uniformPDF() = 0;
    virtual std::vector<T> regularSamples(pcg32 &rng) = 0;
    virtual std::vector<T> jitteredRegularSamples(pcg32 &rng) = 0;
    virtual Float getExtents() const = 0;
    virtual T getStart() const = 0;
    virtual T getEnd() const = 0;
protected:
};

struct Bounds1Df : public Bounds<Float>
{
    Bounds1Df(Float min, Float max) : min(min), max(max) {}

    virtual Float uniformSample(pcg32 &rng)
    {
        return (max - min) * rng.nextDouble() + min;
    }

    virtual Float uniformPDF()
    {
        return 1.0 / (max - min);
    }

    virtual std::vector<Float> regularSamples(pcg32 &rng)
    {
        std::vector<Float> samps = std::vector<Float>();

        throw new NotImplementedException("Bounds 1df regular samples");

        return samps;
    }

    virtual std::vector<Float> jitteredRegularSamples(pcg32 &rng)
    {
        std::vector<Float> samps = std::vector<Float>();

        throw new NotImplementedException("Bounds 1df jittered regular samples");

        return samps;
    }

    virtual Float getExtents() const { return max - min; }
    virtual Float getStart() const { return min; }
    virtual Float getEnd() const { return max; }

protected:
    Float max;
    Float min;
};