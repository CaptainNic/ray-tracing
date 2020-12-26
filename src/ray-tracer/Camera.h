#pragma once

#include "ray.h"
#include "rtmath.h"
#include "vec3.h"

namespace rt {
    class Camera {
    public:
        Camera(
            rt::Point3 lookFrom,
            rt::Point3 lookAt,
            rt::Vec3 viewUp,
            double aspectRatio,
            double verticalFov,
            double aperture,
            double focusDistance)
        {
            // Setup viewport
            auto theta = rt::deg2rad(verticalFov);
            auto h = std::tan(theta / 2);
            auto viewportHeight = 2.0 * h;
            auto viewportWidth = aspectRatio * viewportHeight;

            m_w = (lookFrom - lookAt).unitVector();
            m_u = viewUp.cross(m_w).unitVector();
            m_v = m_w.cross(m_u);

            m_origin = lookFrom;
            m_horizontal = focusDistance * viewportWidth * m_u;
            m_vertical = focusDistance * viewportHeight * m_v;
            m_lowerLeft = m_origin - (m_horizontal/2) - (m_vertical/2) - (focusDistance * m_w);

            m_lensRadius = aperture / 2;
        }

        rt::Ray getRay(double s, double t) const {
            // Generate a random offset on the simulated lens
            // for defocus blur (depth of field).
            auto rd = m_lensRadius * rt::randVecInUnitDisk();
            auto offset = m_u * rd.x() + m_v * rd.y();

            return rt::Ray(
                m_origin + offset,
                m_lowerLeft + (s * m_horizontal) + (t * m_vertical) - m_origin - offset);
        }

    private:
        rt::Point3 m_origin;
        rt::Point3 m_lowerLeft;
        rt::Vec3 m_horizontal;
        rt::Vec3 m_vertical;
        rt::Vec3 m_u;
        rt::Vec3 m_v;
        rt::Vec3 m_w;
        double m_lensRadius;
    };

}
