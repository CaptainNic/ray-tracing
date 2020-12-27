#pragma once

#include "SolidColor.h"
#include "Texture.h"

namespace rt {
namespace textures {
    class Checkered : public ITexture {
    public:
        Checkered()
        {}
        
        Checkered(std::shared_ptr<ITexture> even, std::shared_ptr<ITexture> odd)
            : m_even(even), m_odd(odd)
        {}

        Checkered(color::rgb even, color::rgb odd)
            : m_even(std::make_shared<textures::SolidColor>(even))
            , m_odd(std::make_shared<textures::SolidColor>(odd))
        {}

        // Inherited via ITexture
        virtual color::rgb value(double u, double v, const Point3& p) const override;

    private:
        std::shared_ptr<ITexture> m_even;
        std::shared_ptr<ITexture> m_odd;
    };
}
}