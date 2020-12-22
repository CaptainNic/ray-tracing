#include "Metallic.h"

#include "../hittable.h"
#include "../ray.h"
#include "../vec3.h"

namespace rt {
namespace materials {
    bool Metallic::scatter(const Ray& in, const HitRecord& rec, color::rgb& attenuation, Ray& scattered) const
    {
        // Metallic scattering is a reflection on the normal
        auto reflected = in.direction().unitVector().reflect(rec.normal);
        scattered = Ray(rec.p, reflected);
        attenuation = m_albedo;

        return (dot(scattered.direction(), rec.normal) > 0);
    }
}
}