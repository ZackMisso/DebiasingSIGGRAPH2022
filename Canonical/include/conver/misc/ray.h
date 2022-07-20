#pragma once

#include <conver/conver.h>
#include <conver/misc/vector.h>

class Ray
{
public:
    // Ray Public Methods
    Ray() : tMax(Infinity) {}
    Ray(const Vector3f &o, const Vector3f &d, Float tMax = Infinity)
        : o(o), d(d), tMax(tMax) {}
    Vector3f operator()(Float t) const { return o + d * t; }

    // Ray Public Data
    Vector3f o;
    Vector3f d;
    Float tMax;
};