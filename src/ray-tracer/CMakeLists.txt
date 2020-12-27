add_library(ray-tracer STATIC
    AABB.cc
    BVHNode.cc
    color.cc
    rtmath.cc
    vec3.cc
    HitList.cc
    materials/Dielectric.cc
    materials/Lambertian.cc
    materials/Metallic.cc
    shapes/MovingSphere.cc
    shapes/sphere.cc
    textures/Checkered.cc
    textures/SolidColor.cc
)