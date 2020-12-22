#pragma once

#include "color.h"

namespace rt {
    class Ray;
    struct HitRecord;

    class IMaterial {
    public:
        virtual bool scatter(
            const Ray& input,
            const HitRecord& rec,
            rt::color::rgb& attentuation,
            Ray& scattered) const = 0;
    };
}