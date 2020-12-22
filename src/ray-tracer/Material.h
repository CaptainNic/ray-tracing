#pragma once

namespace rt {
    class Ray;
    struct HitRecord;

    class IMaterial {
    public:
        virtual bool scatter(
            const Ray& input,
            const HitRecord& rec,
            color::rgb& attentuation,
            Ray& scattered) const = 0;
    }
}