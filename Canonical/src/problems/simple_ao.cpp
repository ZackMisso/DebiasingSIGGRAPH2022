#include <conver/problem.h>
#include <conver/misc/sphere.h>

// TODO: get this implemennted

void ConcentricSampleDisk(const Float &ux, const Float &uy, Float &vx, Float &vy)
{
    // Map uniform random numbers to $[-1,1]^2$
    Float uOffsetx = 2.f * ux - 1.f;
    Float uOffsety = 2.f * uy - 1.f;

    // Handle degeneracy at the origin
    if (uOffsetx == 0 && uOffsety == 0)
    {
        vx = 0.0;
        vy = 0.0;
        return;
    }

    // Apply concentric mapping to point
    Float theta, r;
    if (std::abs(uOffsetx) > std::abs(uOffsety))
    {
        r = uOffsetx;
        theta = PiOver4 * (uOffsety / uOffsetx);
    }
    else
    {
        r = uOffsety;
        theta = PiOver2 - PiOver4 * (uOffsetx / uOffsety);
    }

    vx = r * std::cos(theta);
    vy = r * std::sin(theta);
}

inline Vector3f CosineSampleHemisphere(const Float &ux, const Float &uy)
{
    Float dx;
    Float dy;
    ConcentricSampleDisk(ux, uy, dx, dy);
    Float z = std::sqrt(std::max((Float)0, 1 - dx * dx - dy * dy));
    return Vector3f(dx, dy, z);
}

SimpleAO::SimpleAO(Float sphere_height, Float sphere_radius) : sphere_height(sphere_height), sphere_radius(sphere_radius)
{
    // bounds should not really be used here
    // bounds = new SimpleFD(optical_depth->a, optical_depth->b);
}

SimpleAO::~SimpleAO()
{
    // should this object own the function? Probably not
    // delete optical_depth;
}

Float SimpleAO::eval(double sample) const
{
    // sample will incode the distance from pt, so this will return (f(pt+h) - f(pt)) / h
    // return (func->eval(pt + sample) - func->eval(pt)) / sample;
}

Float SimpleAO::eval(const std::vector<Float> &samples) const
{
    // this method requires two samples to sample the hemisphere

    throw new NotImplementedException("simple ao eval samples");
}

Float SimpleAO::expected() const
{
    Vector3f n = Vector3f(0.f, 1.f, 0.f);
    Float radius = sphere_radius;
    Vector3f center = Vector3f(0.0, 0.0, 0.0);
    Vector3f wi = (center - Vector3f(0.0, -sphere_height, 0.0));
    Float dist = std::sqrt(Dot(wi, wi));
    Float norm = Dot(n, wi / dist);
    Float h = radius / dist;
    return 1.0 - h * h * norm;
}