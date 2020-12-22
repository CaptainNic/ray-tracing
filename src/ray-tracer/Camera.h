#pragma once

#include "ray.h"
#include "rtmath.h"
#include "vec3.h"

class Camera {
public:
    Camera(rtmath::Point3& pos, double width, double height)
        : m_origin(pos)
    {
        auto aspectRatio = width / height;
        auto viewportHeight = 2.0;
        auto viewportWidth = aspectRatio * viewportHeight;
        auto focalLength = 1.0;

        m_horizontal = rtmath::Vec3(viewportWidth, 0, 0);
        m_vertical = rtmath::Vec3(0, viewportHeight, 0);
        m_lowerLeft = m_origin - (m_horizontal/2) - (m_vertical/2) - rtmath::Vec3(0, 0, focalLength);
    }

    rtmath::Ray getRay(double u, double v) const {
        return rtmath::Ray(m_origin, m_lowerLeft + (u*m_horizontal) + (v*m_vertical) - m_origin);
    }

private:
    rtmath::Point3 m_origin;
    rtmath::Point3 m_lowerLeft;
    rtmath::Vec3 m_horizontal;
    rtmath::Vec3 m_vertical;
};
