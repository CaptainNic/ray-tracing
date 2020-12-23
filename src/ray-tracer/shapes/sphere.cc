#include "sphere.h"

using rt::HitRecord;
using rt::Ray;
using rt::Vec3;

namespace rt {
namespace shapes {
    bool Sphere::hit(const Ray& r, double tMin, double tMax, HitRecord& rec) const {
        // ax^2 + bx + c = 0    OR    x = (-b +- sqrt(b^2-4ac))/2a
        // As per the quadratic equation:
        // The discriminant (b^2 - 4ac) must be non-negative for there to be
        // a solution since you can't get a real number by sqrt of a negative.
        // Additionally, if the discriminant is 0, the Ray hits the sphere in
        // only one location, thus it is a tangent.
        // If the discriminant is positive, the Ray intersects 2 points of the
        // sphere. We consider this a hit.
        Vec3 oc = r.origin() - m_center;
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
        auto outwardNormal = (rec.p - m_center) / m_radius;
        rec.setFaceNormal(r, outwardNormal);
        rec.material = m_material;
    
        return true;
    }
} // namespace shapes
} // namespace rt