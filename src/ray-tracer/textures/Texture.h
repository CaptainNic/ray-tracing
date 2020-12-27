#pragma once

#include "../color.h"

namespace rt {
namespace textures {
    class ITexture {
    public:
        virtual color::rgb value(double u, double v, const Point3& p) const = 0;
    };
}
}
