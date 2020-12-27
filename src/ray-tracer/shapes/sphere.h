#pragma once

#include "../materials/Material.h"
#include "../hittable.h"
#include "../vec3.h"

namespace rt {
namespace shapes {
    class Sphere : public rt::IHittable {
    public:
        Sphere(rt::Point3 cen, double r, std::shared_ptr<IMaterial> mat)
            : m_center(cen), m_radius(r), m_material(mat)
        {};

        // Inherited via IHittable
        virtual bool hit(const rt::Ray& r, double tMin, double tMax, rt::HitRecord& rec) const override;
        virtual bool boundingBox(double t0, double t1, AABB& outBox) const override;

        rt::Point3 center() const { return m_center; }
        double radius() const { return m_radius; }

    private:
        static void getUV(const Point3& p, double& u, double& v);

        rt::Point3 m_center;
        double m_radius;
        std::shared_ptr<IMaterial> m_material;

    };
} // namespace shapes
} // namespace rt
