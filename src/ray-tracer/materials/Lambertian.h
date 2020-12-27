#pragma once

#include "../color.h"
#include "Material.h"
#include "../textures/SolidColor.h"
#include "../textures/Texture.h"

namespace rt {
namespace materials {
    class Lambertian : public IMaterial {
    public:
        Lambertian(const color::rgb& a) : m_albedo(std::make_shared<textures::SolidColor>(a)) {}
        Lambertian(std::shared_ptr<textures::ITexture> a) : m_albedo(a) {}

        virtual bool scatter(
            const Ray& in, const HitRecord& rec, color::rgb& attenuation, Ray& scattered) const override;

    private:
        std::shared_ptr<textures::ITexture> m_albedo;
    };
} // namespace materials
} // namespace rt