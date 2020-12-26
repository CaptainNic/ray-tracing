#include "MovingSphere.h"

namespace rt {
namespace shapes {

    bool MovingSphere::hit(const Ray& r, double tMin, double tMax, HitRecord& rec) const
    {
        // This is copy pasta from Sphere with minor changes to calculate center
        // and normal while accounting for time.
        auto centerNow = center(r.time());
        auto oc = r.origin() - centerNow;
        auto a = r.direction().length_squared();
        auto halfB = rt::dot(oc, r.direction());
        auto c = oc.length_squared() - (m_radius * m_radius);

        auto discriminant = (halfB * halfB) - a * c;
        if (discriminant < 0) {
            return false;
        }

        auto sqrtd = std::sqrt(discriminant);

        // Find nearest root in acceptable range
        auto root = (-halfB - sqrtd) / a;
        if (root < tMin || root > tMax) {
            root = (-halfB + sqrtd) / a;
            if (root < tMin || root > tMax) {
                return false;
            }
        }

        rec.t = root;
        rec.p = r.at(rec.t);
        auto outwardNormal = (rec.p - centerNow) / m_radius;
        rec.setFaceNormal(r, outwardNormal);
        rec.material = m_material;

        return true;
    }

    Point3 MovingSphere::center(double time) const
    {
        auto dt = time - m_timeStart;
        auto timeScale = dt / m_travelTime;

        return m_centerStart + (m_travelVector * timeScale);
    }
}
}

