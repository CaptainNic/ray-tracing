#pragma once

#include <memory>
#include "AABB.h"
#include "materials/Material.h"
#include "ray.h"

namespace rt {
    struct HitRecord {
        Point3 p;
        Vec3 normal;
        std::shared_ptr<IMaterial> material;
        double t;

        // Surface coordinates of the ray hit point.
        double u;
        double v;

        // Whether the ray hit the front or back face of the object.
        bool frontFace;

        HitRecord()
            : p(Point3(0, 0, 0))
            , normal(Vec3(0, 0, 0))
            , t(0.0), u(0.0), v(0.0)
            , frontFace(false)
        {}

        inline void setFaceNormal(const Ray& r, const Vec3& outwardNormal) {
            frontFace = dot(r.direction(), outwardNormal) < 0;
            normal = frontFace ? outwardNormal : -outwardNormal;
        }
    };

    class IHittable {
    public:
        virtual bool hit(const Ray& r, double tMin, double tMax, HitRecord& rec) const = 0;
        virtual bool boundingBox(double t0, double t1, AABB& outBox) const = 0;
    };
}