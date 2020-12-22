#pragma once

#include "../hittable.h"
#include "../vec3.h"

namespace rtshapes {
    class Sphere : public rt::IHittable {
    public:
        Sphere(rt::Point3 cen, double r) : m_center(cen), m_radius(r) {};

        virtual bool hit(const rt::Ray& r, double tMin, double tMax, rt::HitRecord& rec) const override;

        rt::Point3 center() const { return m_center; }
        double radius() const { return m_radius; }

    private:
        rt::Point3 m_center;
        double m_radius;
    };
}
