#pragma once

#include "../hittable.h"

namespace rt {
namespace shapes {
    class MovingSphere : public IHittable {
    public:
        MovingSphere()
            : m_timeStart(0), m_timeEnd(0), m_radius(0)
        {}

        MovingSphere(Point3 centerStart, Point3 centerEnd, double timeStart, double timeEnd, double r, std::shared_ptr<IMaterial> material)
            : m_centerStart(centerStart), m_centerEnd(centerEnd)
            , m_timeStart(timeStart), m_timeEnd(timeEnd)
            , m_radius(r), m_material(material)
        {}

        // Inherited via IHittable
        virtual bool hit(const Ray& r, double tMin, double tMax, HitRecord& rec) const override;
        virtual bool boundingBox(double t0, double t1, AABB& outBox) const override;

        Point3 center(double time) const;

    private:
        Point3 m_centerStart;
        Point3 m_centerEnd;
        double m_timeStart;
        double m_timeEnd;
        double m_radius;
        std::shared_ptr<IMaterial> m_material;
    };
}
}