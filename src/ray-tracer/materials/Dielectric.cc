#include "Dielectric.h"

#include "../hittable.h"
#include "../ray.h"
#include "../vec3.h"

namespace rt {
namespace materials {
    bool Dielectric::scatter(const Ray& in, const HitRecord& rec, color::rgb& attenuation, Ray& scattered) const
    {
        attenuation = color::rgb(1.0, 1.0, 1.0);

        // Ratio is flipped depending on if we're entering or leaving the material.
        double refractionRatio = rec.frontFace ? (1.0 / m_refractionIndex) : m_refractionIndex;

        auto unitDirection = in.direction().unitVector();
        auto cosTheta = std::fmin((-unitDirection).dot(rec.normal), 1.0);
        auto sinTheta = std::sqrt(1.0 - cosTheta * cosTheta);
        

        // Check if refraction is possible (ratio * sin(theta) <= 1.0), then
        // simulate real glass at high angles reflection.
        bool shouldReflect = (refractionRatio * sinTheta > 1.0)
            || (reflectance(cosTheta, refractionRatio) > randDouble());

        Vec3 direction;
        if (shouldReflect) {
            // Refraction impossible, so we reflect.
            auto reflected = unitDirection.reflect(rec.normal);
            scattered = Ray(rec.p, reflected);
        } else {
            auto refracted = unitDirection.refract(rec.normal, refractionRatio);
            scattered = Ray(rec.p, refracted);
        }

        return true;
    }

    double Dielectric::reflectance(double cos, double refIdx)
    {
        // Use Schlick's approximation for reflectance.
        // https://en.wikipedia.org/wiki/Schlick%27s_approximation
        // R(theta) = R0 + (1-R0)(1-cos(theta)^5
        // R0 = ((n1-n2)/(n1+n2))^2
        // n1,n2 = refractive indices of the two materials, but we hardcode n1 = 1
        //      because we always assume it's a vacuum.
        auto r0 = (1 - refIdx) / (1 + refIdx);
        r0 = r0 * r0;
        auto reflectance = r0 + (1 - r0) * std::pow((1 - cos), 5);
        return reflectance;
    }
}
}