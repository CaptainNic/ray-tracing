#pragma once

#include <cmath>
#include <limits>
#include <memory>
#include <random>

// Constants & Helpers
namespace rtmath {
    const double infinity = std::numeric_limits<double>::infinity();
    const double pi = 3.1415926535897932385;

    inline double deg2rad(double deg) { return deg * pi / 180.0; }

    inline double randDouble() {
        static std::uniform_real_distribution<double> distribution(0.0, 1.0);
        static std::mt19937 generator;
        return distribution(generator);
    }
    
    inline double randDouble(double min, double max) { return min + (max - min) * randDouble(); }

    inline double clamp(double x, double min, double max) {
        if (x < min) return min;
        if (x > max) return max;
        return x;
    }
}
