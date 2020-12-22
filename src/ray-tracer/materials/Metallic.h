#pragma once

#include "../color.h"
#include "../Material.h"

namespace rt {
namespace materials {
    class Metallic : public IMaterial {
    public:
        Metallic(const color::rgb& a) : m_albedo(a) {}

        virtual bool scatter(
            const Ray& in, const HitRecord& rec, color::rgb& attenuation, Ray& scattered) const override;

    private:
        color::rgb m_albedo;
    };
}
}