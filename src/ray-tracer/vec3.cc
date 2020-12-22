#include "vec3.h"

namespace rt {
    Vec3 randVecInUnitSphere() {
        while (true) {
            auto p = Vec3::random(-1, 1);
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

    Vec3 Vec3::reflect(const Vec3& n) const
    {
        return *this - (2 * dot(*this, n) * n);
    }
}

