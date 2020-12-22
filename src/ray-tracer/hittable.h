#pragma once

#include <memory>
#include "Material.h"
#include "ray.h"

namespace rt {
    struct HitRecord {
        Point3 p;
        Vec3 normal;
        std::shared_ptr<IMaterial> material;
        double t;
        bool frontFace;

        HitRecord() : p(Point3(0, 0, 0)), normal(Vec3(0, 0, 0)), t(0.0), frontFace(false) {}

        inline void setFaceNormal(const Ray& r, const Vec3& outwardNormal) {
            frontFace = dot(r.direction(), outwardNormal) < 0;
            normal = frontFace ? outwardNormal : -outwardNormal;
        }
    };

    class IHittable {
    public:
        virtual bool hit(const Ray& r, double tMin, double tMax, HitRecord& rec) const = 0;
    };
}