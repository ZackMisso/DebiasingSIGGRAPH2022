#pragma once

#include <conver/conver.h>
#include <conver/misc/ray.h>

// struct Sphere
// {
//     Sphere(Float height, Float radius) : height(height), radius(radius) {}

//     Float height;
//     Float radius;
// };

/*
    pbrt source code is Copyright(c) 1998-2016
                        Matt Pharr, Greg Humphreys, and Wenzel Jakob.

    This file is part of pbrt.

    Redistribution and use in source and binary forms, with or without
    modification, are permitted provided that the following conditions are
    met:

    - Redistributions of source code must retain the above copyright
      notice, this list of conditions and the following disclaimer.

    - Redistributions in binary form must reproduce the above copyright
      notice, this list of conditions and the following disclaimer in the
      documentation and/or other materials provided with the distribution.

    THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS
    IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED
    TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A
    PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
    HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
    SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
    LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
    DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
    THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
    (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
    OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

 */

// Sphere Declarations
struct Sphere
{
    // public:
    // Sphere Public Methods
    Sphere(Float radius)
        : radius(radius),
          zMin(-radius),
          zMax(radius),
          thetaMin(-1),
          thetaMax(1),
          phiMax(Radians(360))
    {
    }
    // Bounds3f ObjectBound() const;
    bool intersect(const Ray &ray) const;
    // Float Area() const;
    // Interaction Sample(const Point2f &u, Float *pdf) const;
    // Interaction Sample(const Interaction &ref, const Point2f &u,
    //                    Float *pdf) const;
    // Float Pdf(const Interaction &ref, const Vector3f &wi) const;
    // Float SolidAngle(const Point3f &p, int nSamples) const;

    // bool isSphere() const { return true; }

    // private:
    // Sphere Private Data
    Float radius;
    Float zMin, zMax;
    Float thetaMin, thetaMax, phiMax;
};