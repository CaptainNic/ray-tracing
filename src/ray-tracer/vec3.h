#pragma once

#include <cmath>
#include <iostream>
#include "rtmath.h"

namespace rtmath {
    class Vec3
    {
    public:
        Vec3() : e{0,0,0} {}
        Vec3 (double e0, double e1, double e2) : e{e0, e1, e2} {}

        double x() const { return e[0]; }
        double y() const { return e[1]; }
        double z() const { return e[2]; }

        Vec3 operator-() const { return Vec3(-e[0], -e[1], -e[2]); }
        double operator[](int i) const { return e[i]; }
        double& operator[](int i) { return e[i]; }
        Vec3& operator+=(const Vec3& v) { e[0] += v[0]; e[1] += v[1]; e[2] += v[2]; return *this; }
        Vec3& operator*=(const double t) { e[0] *= t; e[1] *= t; e[2] *= t; return *this; }
        Vec3& operator/=(const double t) { return *this *= 1/t; }

        double length() const { return std::sqrt(length_squared()); }
        double length_squared() const { return e[0]*e[0] + e[1]*e[1] + e[2]*e[2]; }

        inline static Vec3 random() {
            return Vec3(randDouble(), randDouble(), randDouble());
        }

        inline static Vec3 random(double min, double max) {
            return Vec3(randDouble(min, max), randDouble(min, max), randDouble(min, max));
        }

    private:
        double e[3];
    };

    // Aliases
    using Point3 = Vec3;

    // Vec3 utility functions
    inline std::ostream& operator<< (std::ostream& out, const Vec3& v) { return out << "(" << v.x() << ", " << v.y() << ", " << v.z() << ")"; }

    inline Vec3 operator+ (const Vec3& u, const Vec3& v) { return Vec3(u.x() + v.x(), u.y() + v.y(), u.z() + v.z()); }
    inline Vec3 operator- (const Vec3& u, const Vec3& v) { return Vec3(u.x() - v.x(), u.y() - v.y(), u.z() - v.z()); }
    inline Vec3 operator* (const Vec3& u, const Vec3& v) { return Vec3(u.x() * v.x(), u.y() * v.y(), u.z() * v.z()); }
    inline Vec3 operator* (const double t, const Vec3& v) { return Vec3(t * v.x(), t * v.y(), t * v.z()); }
    inline Vec3 operator* (const Vec3& v, const double t) { return t * v; }
    inline Vec3 operator/ (const Vec3& v, const double t) { return (1/t) * v; }
    inline double dot(const Vec3& u, const Vec3& v) { return u.x() * v.x() + u.y() * v.y() + u.z() * v.z(); }
    inline Vec3 cross(const Vec3& u, const Vec3& v) {
        return Vec3(u.y() * v.z() - u.z() * v.y(),
                    u.z() * v.x() - u.x() * v.z(),
                    u.x() * v.y() - u.y() * v.x());
    }
    inline Vec3 unit_vector(const Vec3 v) { return v / v.length(); }

    /// <summary>
    /// Generates a random vector within a unit sphere.
    /// May the RNG be on your side.
    /// </summary>
    /// <returns>A Vec3 within a unit sphere.</returns>
    Vec3 randVecInUnitSphere();
}
