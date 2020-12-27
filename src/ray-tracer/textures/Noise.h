#pragma once

#include "../Perlin.h"
#include "Texture.h"

namespace rt {
namespace textures {

    class Noise : public ITexture {
    public:
        Noise() : Noise(1.0) {}
        Noise(double scale) : m_scale(scale) {}

        virtual color::rgb value(double u, double v, const Point3& p) const override {
            // Perlin noise returns values in range of [-1, 1], which causes problems
            // with gamma correction (sqrt of negative number is NaN), so we adjust
            // the values to the range [0, 1].
            // auto noiseValue = 0.5 * (1.0 + m_noise.noise(m_scale * p));
            
            // Turbulence is in the range of [0, 1] and doesn't need adjustment.
            // auto noiseValue = 0.5 * m_noise.turbulence(m_scale * p);
            
            // When used indirectly, you can get a marble-like texture by phasing the turbulence
            // in and out.
            // Again, we need to adjust values from [-1, 1] -> [0, 1].
            auto noiseValue = 0.5 * (1 + std::sin((m_scale * p.z()) + (10 * m_noise.turbulence(p))));

            return color::rgb(1, 1, 1) * noiseValue;
        }

    private:
        Perlin<256> m_noise;
        double m_scale;
    };
}
}