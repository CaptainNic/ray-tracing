#include "ray.h"
#include "vec3.h"

namespace rt {

    /// <summary>
    /// Axis-Aligned Bounding Box
    /// </summary>
    class AABB {
    public:
        AABB() {}
        AABB(const Point3& a, const Point3& b) : m_min(a), m_max(b) {}

        Point3 min() const { return m_min; }
        Point3 max() const { return m_max; }

        bool hit(const Ray& r, double tMin, double tMax) const;

        static AABB surroundingBox(AABB a, AABB b);

    private:
        Point3 m_min;
        Point3 m_max;
    };
}