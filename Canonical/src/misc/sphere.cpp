#include <conver/misc/sphere.h>
#include <conver/misc/efloat.h>

bool Sphere::intersect(const Ray &r) const
{
    Float phi;
    Vector3f pHit;
    // Transform _Ray_ to object space
    Vector3f oErr, dErr;
    // Ray ray = (*WorldToObject)(r, &oErr, &dErr);
    Ray ray = r; // transform between world and object is assumed to be the identity matrix

    // Compute quadratic sphere coefficients

    // Initialize _EFloat_ ray coordinate values
    EFloat ox(ray.o.x, oErr.x), oy(ray.o.y, oErr.y), oz(ray.o.z, oErr.z);
    EFloat dx(ray.d.x, dErr.x), dy(ray.d.y, dErr.y), dz(ray.d.z, dErr.z);
    EFloat a = dx * dx + dy * dy + dz * dz;
    EFloat b = 2 * (dx * ox + dy * oy + dz * oz);
    EFloat c = ox * ox + oy * oy + oz * oz - EFloat(radius) * EFloat(radius);

    // Solve quadratic equation for _t_ values
    EFloat t0, t1;
    if (!Quadratic(a, b, c, &t0, &t1))
        return false;

    // Check quadric shape _t0_ and _t1_ for nearest intersection
    if (t0.UpperBound() > ray.tMax || t1.LowerBound() <= 0)
        return false;
    EFloat tShapeHit = t0;
    if (tShapeHit.LowerBound() <= 0)
    {
        tShapeHit = t1;
        if (tShapeHit.UpperBound() > ray.tMax)
            return false;
    }

    // Compute sphere hit position and $\phi$
    pHit = ray((Float)tShapeHit);

    // Refine sphere intersection point
    pHit *= radius / Distance(pHit, Vector3f(0, 0, 0));
    if (pHit.x == 0 && pHit.y == 0)
        pHit.x = 1e-5f * radius;
    phi = std::atan2(pHit.y, pHit.x);
    if (phi < 0)
        phi += 2 * Pi;

    // Test sphere intersection against clipping parameters
    if ((zMin > -radius && pHit.z < zMin) || (zMax < radius && pHit.z > zMax) ||
        phi > phiMax)
    {
        if (tShapeHit == t1)
            return false;
        if (t1.UpperBound() > ray.tMax)
            return false;
        tShapeHit = t1;
        // Compute sphere hit position and $\phi$
        pHit = ray((Float)tShapeHit);

        // Refine sphere intersection point
        pHit *= radius / Distance(pHit, Vector3f(0, 0, 0));
        if (pHit.x == 0 && pHit.y == 0)
            pHit.x = 1e-5f * radius;
        phi = std::atan2(pHit.y, pHit.x);
        if (phi < 0)
            phi += 2 * Pi;
        if ((zMin > -radius && pHit.z < zMin) ||
            (zMax < radius && pHit.z > zMax) || phi > phiMax)
            return false;
    }
    return true;
}