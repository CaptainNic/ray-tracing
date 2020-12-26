#pragma once

#include "../hittable.h"

namespace rt {
namespace shapes {
    class MovingSphere : public IHittable {
    public:
        MovingSphere()
            : m_timeStart(0), m_travelTime(0), m_radius(0)
        {}

        MovingSphere(Point3 centerStart, Point3 centerEnd, double timeStart, double timeEnd, double r, std::shared_ptr<IMaterial> material)
            : m_centerStart(centerStart), m_travelVector(centerEnd - centerStart)
            , m_timeStart(timeStart), m_travelTime(timeEnd - timeStart)
            , m_radius(r), m_material(material)
        {}

        virtual bool hit(const Ray& r, double tMin, double tMax, HitRecord& rec) const override;

        Point3 center(double time) const;

    private:
        Point3 m_centerStart;
        Vec3 m_travelVector;
        double m_timeStart;
        double m_travelTime;
        double m_radius;
        std::shared_ptr<IMaterial> m_material;
    };
}
}