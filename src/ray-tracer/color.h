#pragma once

#include <iostream>
#include "rtmath.h"

namespace color {
    // Alias for color data
    using rgb = rtmath::Vec3;
 
    void write(std::ostream& out, rgb px_color) {
        // Translate to [0 - 255] range
        out << static_cast<int>(255.999 * px_color.x()) << ' '
            << static_cast<int>(255.999 * px_color.y()) << ' '
            << static_cast<int>(255.999 * px_color.z()) << std::endl;
    }

    void write(std::ostream& out, rgb px_color, int numSamples) {
        auto r = px_color.x();
        auto g = px_color.y();
        auto b = px_color.z();

        // When multisampling, we just keep adding each sample to the color,
        // so all we need to do here is average them by the number of samples.
        auto scale = 1.0 / numSamples;
        r *= scale;
        g *= scale;
        b *= scale;

        // We also do some gamma correction (gamma 2) by raising the color value
        // by the power of 1/gamma. So, gamma 2 --> sqrt(value).
        r = std::sqrt(r);
        g = std::sqrt(g);
        b = std::sqrt(b);

        out << static_cast<int>(255.999 * rtmath::clamp(r, 0.0, 0.999)) << ' '
            << static_cast<int>(255.999 * rtmath::clamp(g, 0.0, 0.999)) << ' '
            << static_cast<int>(255.999 * rtmath::clamp(b, 0.0, 0.999)) << std::endl;
    }
}