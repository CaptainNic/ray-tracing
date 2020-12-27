#include "Checkered.h"

namespace rt {
namespace textures {
    color::rgb Checkered::value(double u, double v, const Point3& p) const
    {
        auto sines = std::sin(10 * p.x()) * std::sin(10 * p.y()) * std::sin(10 * p.z());
        return (sines < 0)
            ? m_odd->value(u, v, p)
            : m_even->value(u, v, p);
    }
}
}