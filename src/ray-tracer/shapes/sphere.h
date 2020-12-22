#pragma once

#include "../hittable.h"
#include "../vec3.h"

namespace rtshapes {
    class Sphere : public rtmath::IHittable {
    public:
        Sphere(rtmath::Point3 cen, double r) : m_center(cen), m_radius(r) {};

        virtual bool hit(const rtmath::Ray& r, double tMin, double tMax, rtmath::HitRecord& rec) const override;

        rtmath::Point3 center() const { return m_center; }
        double radius() const { return m_radius; }

    private:
        rtmath::Point3 m_center;
        double m_radius;
    };
}
