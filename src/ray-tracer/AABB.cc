#include "AABB.h"

namespace rt {
    bool AABB::hit(const Ray& r, double tMin, double tMax) const {
        for (int axis = 0; axis < 3; axis++) {
            auto inverseDir = 1.0f / r.direction()[axis];
            auto t0 = (min()[axis] - r.origin()[axis]) * inverseDir;
            auto t1 = (max()[axis] - r.origin()[axis]) * inverseDir;
            
            // Swap so t0 is smaller, makes math comparison simpler.
            if (inverseDir < 0.0f) {
                std::swap(t0, t1);
            }

            // Tighten bounds so we're only looking at overlap.
            tMin = (t0 > tMin) ? t0 : tMin;
            tMax = (t1 < tMax) ? t1 : tMax;

            // Early out if there is no overlap on this axis.
            if (tMax <= tMin) {
                return false;
            }
        }

        return false;
    }

    AABB AABB::surroundingBox(AABB a, AABB b)
    {
        Point3 small(
            std::fmin(a.min().x(), b.min().x()),
            std::fmin(a.min().y(), b.min().y()),
            std::fmin(a.min().z(), b.min().z()));

        Point3 big(
            std::fmax(a.max().x(), b.max().x()),
            std::fmax(a.max().y(), b.max().y()),
            std::fmax(a.max().z(), b.max().z()));

        return AABB(small, big);
    }
}