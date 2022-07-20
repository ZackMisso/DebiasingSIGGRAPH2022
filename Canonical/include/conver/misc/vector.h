#pragma once

#include <conver/conver.h>

struct Vector3f
{
    // Vector3 Public Methods
    Float operator[](int i) const
    {
        if (i == 0)
            return x;
        if (i == 1)
            return y;
        return z;
    }
    Float &operator[](int i)
    {
        if (i == 0)
            return x;
        if (i == 1)
            return y;
        return z;
    }
    Vector3f() { x = y = z = 0; }
    Vector3f(Float x, Float y, Float z) : x(x), y(y), z(z)
    {
    }
    // bool HasNaNs() const { return isNaN(x) || isNaN(y) || isNaN(z); }
    // The default versions of these are fine for release builds; for debug
    // we define them so that we can add the Assert checks.
    Vector3f(const Vector3f &v)
    {
        x = v.x;
        y = v.y;
        z = v.z;
    }

    Vector3f &operator=(const Vector3f &v)
    {
        x = v.x;
        y = v.y;
        z = v.z;
        return *this;
    }
    Vector3f operator+(const Vector3f &v) const
    {
        return Vector3f(x + v.x, y + v.y, z + v.z);
    }
    Vector3f &operator+=(const Vector3f &v)
    {
        x += v.x;
        y += v.y;
        z += v.z;
        return *this;
    }
    Vector3f operator-(const Vector3f &v) const
    {
        return Vector3f(x - v.x, y - v.y, z - v.z);
    }
    Vector3f &operator-=(const Vector3f &v)
    {
        x -= v.x;
        y -= v.y;
        z -= v.z;
        return *this;
    }
    bool operator==(const Vector3f &v) const
    {
        return x == v.x && y == v.y && z == v.z;
    }
    bool operator!=(const Vector3f &v) const
    {
        return x != v.x || y != v.y || z != v.z;
    }
    Vector3f operator*(Float s) const
    {
        return Vector3f(s * x, s * y, s * z);
    }
    Vector3f &operator*=(Float s)
    {
        x *= s;
        y *= s;
        z *= s;
        return *this;
    }
    Vector3f operator/(Float f) const
    {
        if (f == 0)
            return Vector3f(0, 0, 0); // nooo
        Float inv = (Float)1 / f;
        return Vector3f(x * inv, y * inv, z * inv);
    }
    Float absSum() const
    {
        return std::abs(x) + std::abs(y) + std::abs(z);
    }
    Vector3f &operator/=(Float f)
    {
        Float inv = (Float)1 / f;
        x *= inv;
        y *= inv;
        z *= inv;
        return *this;
    }
    Vector3f operator-() const { return Vector3f(-x, -y, -z); }
    Float LengthSquared() const { return x * x + y * y + z * z; }
    Float Length() const { return std::sqrt(LengthSquared()); }

    // Vector3 Public Data
    Float x, y, z;
};

inline Float Distance(const Vector3f &p1, const Vector3f &p2)
{
    return (p1 - p2).Length();
}

inline Float Dot(const Vector3f &p1, const Vector3f &p2)
{
    return (p1.x * p2.x + p1.y * p2.y + p1.z * p2.z);
}