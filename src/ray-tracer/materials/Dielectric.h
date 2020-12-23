#pragma once

#include "../color.h"
#include "../Material.h"

namespace rt {
namespace materials {
    class Dielectric : public IMaterial {
    public:
        Dielectric(double ir) : m_refractionIndex(ir) {}

        virtual bool scatter(
            const Ray& in, const HitRecord& rec, color::rgb& attenuation, Ray& scattered) const override;

    private:
        static double reflectance(double cos, double refIdx);

        double m_refractionIndex;
    };
}
}