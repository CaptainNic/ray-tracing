#include "vec3.h"

#include <cmath>

namespace rt {
    Vec3 randVecInUnitSphere() {
        while (true) {
            auto p = Vec3::random(-1, 1);
            if (p.length_squared() < 1) {
                return p;
            }
        }
    }

    Vec3 randVecInUnitDisk()
    {
        while (true) {
            auto p = Vec3(randDouble(-1, 1), randDouble(-1, 1), 0);
            if (p.length_squared() < 1) {
                return p;
            }
        }
    }

    Vec3 randUnitVector()
    {
        return unit_vector(randVecInUnitSphere());
    }

    Vec3 Vec3::unitVector() const
    {
        return unit_vector(*this);
    }

    double Vec3::dot(const Vec3& v) const {
        return rt::dot(*this, v);
    }

    Vec3 Vec3::cross(const Vec3& v) const {
        return rt::cross(*this, v);
    }

    Vec3 Vec3::reflect(const Vec3& n) const
    {
        return *this - (2 * this->dot(n) * n);
    }

    Vec3 Vec3::refract(const Vec3& n, double refractionRatio) const {
        auto inverse = -(*this);
        auto cosTheta = std::fmin(inverse.dot(n), 1.0);
        auto rOutPerpendicular = refractionRatio * (*this + (cosTheta * n));
        auto rOutParallel = -std::sqrt(std::fabs(1.0 - rOutPerpendicular.length_squared())) * n;
        return rOutPerpendicular + rOutParallel;
    }
}

