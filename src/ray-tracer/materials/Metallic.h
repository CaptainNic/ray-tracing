#pragma once

#include "../color.h"
#include "Material.h"

namespace rt {
namespace materials {
    class Metallic : public IMaterial {
    public:
        Metallic(const color::rgb& a, double f) : m_albedo(a), m_fuzz(clamp(f, 0.0, 1.0)) {}

        virtual bool scatter(
            const Ray& in, const HitRecord& rec, color::rgb& attenuation, Ray& scattered) const override;

    private:
        color::rgb m_albedo;
        double m_fuzz;
    };
}
}