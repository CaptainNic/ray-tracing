#pragma once

#include <iostream>
#include "rtmath.h"
#include "vec3.h"

namespace rt {
namespace color {
    // Alias for color data
    using rgb = rt::Vec3;
 
    /// <summary>
    /// Single sample color writer.
    /// </summary>
    /// <param name="out">The output stream.</param>
    /// <param name="px_color">The color value.</param>
    void write(std::ostream& out, rgb px_color);

    /// <summary>
    /// Multi-sampling color writer.
    /// </summary>
    /// <param name="out">The output stream.</param>
    /// <param name="px_color">The multi-sampled color value (sum of all samples).</param>
    /// <param name="numSamples">The number of samples in px_color.</param>
    void write(std::ostream& out, rgb px_color, int numSamples);
} // namespace color
} // namespace rt
