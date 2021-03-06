#pragma once

#include <cmath>
#include <limits>
#include <memory>
#include <random>
#include <thread>

// Constants & Helpers
namespace rt {
    template<typename Unit, Unit min, Unit max>
    class RNG {
    public:
        RNG() {
            std::random_device rd;
            m_dist = std::uniform_real_distribution<Unit>(min, max);
            m_gen = std::mt19937{ rd() };
        }

        double generate() { return m_dist(m_gen); }

    private:
        std::uniform_real_distribution<Unit> m_dist;
        std::mt19937 m_gen;
    };

    const double infinity = std::numeric_limits<double>::infinity();
    const double pi = 3.1415926535897932385;

    inline double deg2rad(double deg) { return deg * pi / 180.0; }

    double randDouble();
    
    inline double randDouble(double min, double max) { return min + (max - min) * randDouble(); }
    inline int randInt(int min, int max) { return static_cast<int>(randDouble(min, max + 1)); }

    inline double clamp(double x, double min, double max) {
        if (x < min) return min;
        if (x > max) return max;
        return x;
    }
}
