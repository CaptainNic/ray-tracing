#include "vec3.h"

namespace rtmath {
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
}

