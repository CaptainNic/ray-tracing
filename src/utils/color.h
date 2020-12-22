#pragma once

#include <iostream>
#include "vec3.h"

namespace color {
    // Alias for color data
    using rgb = rtmath::vec3;
 
    void write(std::ostream& out, rgb px_color) {
        // Translate to [0 - 255] range
        out << static_cast<int>(255.999 * px_color.x()) << ' '
            << static_cast<int>(255.999 * px_color.y()) << ' '
            << static_cast<int>(255.999 * px_color.z()) << std::endl;
    }
}
