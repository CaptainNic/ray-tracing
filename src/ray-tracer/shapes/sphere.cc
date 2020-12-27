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
        getUV(outwardNormal, rec.u, rec.v);
        rec.material = m_material;
    
        return true;
    }

    bool Sphere::boundingBox(double t0, double t1, AABB& outBox) const
    {
        outBox = AABB(
            m_center - Vec3(m_radius, m_radius, m_radius),
            m_center + Vec3(m_radius, m_radius, m_radius));

        return true;
    }
    
    void Sphere::getUV(const Point3& p, double& u, double& v)
    {
        // p: a point on the surface of unit sphere, centered at the origin.
        // u: returned value [0, 1] of the longitudinal position on the sphere.
        // v: returned value [0, 1] of the latitudinal position on the sphere.
        // (0, 0) maps to bottom left of texture

        auto theta = std::acos(-p.y());
        auto phi = std::atan2(-p.z(), p.x()) + rt::pi;

        u = phi / (2 * rt::pi);
        v = theta / pi;
    }
} // namespace shapes
} // namespace rt