#pragma once

#include "Texture.h"

namespace rt {
namespace textures {
    class SolidColor : public ITexture {
    public:
        SolidColor()
        {}
        
        SolidColor(color::rgb c)
            : m_color(c)
        {}
        
        SolidColor(double r, double g, double b)
            : SolidColor(color::rgb(r, g, b))
        {}

        // Inherited via ITexture
        virtual color::rgb value(double u, double v, const Point3& p) const override {
            return m_color;
        }

    private:
        color::rgb m_color;
    };
}
}