#include "Lambertian.h"

#include "../hittable.h"
#include "../ray.h"
#include "../vec3.h"

namespace rt {
namespace materials {
    bool Lambertian::scatter(const Ray& in, const HitRecord& rec, color::rgb& attenuation, Ray& scattered) const
    {
        // Lambertian (diffuse) scattering can scatter in one of two ways:
        // 1. scatter and attenuate by it's reflectance R
        // 2. scatter w/o attentuation but absorb (1-R) of the rays
        // 3. A mixture of 1 and 2

        // Potential that this ends up being 0 if vectors are inverse of each other.
        // In this scenario, we scatter in the normal direction.
        auto scatterDir = rec.normal + randUnitVector();
        if (scatterDir.nearZero()) {
            scatterDir = rec.normal;
        }

        scattered = Ray(rec.p, scatterDir);
        attenuation = m_albedo;

        return true;
    }
}
}