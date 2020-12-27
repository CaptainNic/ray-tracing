#pragma once
#include <functional>
#include <unordered_map>

#include "Camera.h"
#include "color.h"
#include "HitList.h"
#include "rtmath.h"
#include "materials/Dielectric.h"
#include "materials/Lambertian.h"
#include "materials/Metallic.h"
#include "shapes/MovingSphere.h"
#include "shapes/sphere.h"
#include "textures/Checkered.h"
#include "textures/Noise.h"

class Scene {
public:
    Scene() : camera(rt::Point3(0, 0, 1), rt::Point3(0, 0, -1), rt::Vec3(0, 1, 0), 16.0 / 9.0, 60, 0, 1) {}

    rt::HitList world;
    rt::Camera camera;
};

class RandomScene : public Scene {
public:
    RandomScene(double aspectRatio, double timeStart = 0.0, double timeEnd = 0.0) {
        auto checkerTex = std::make_shared<rt::textures::Checkered>(rt::color::rgb(0.2, 0.3, 0.1), rt::color::rgb(0.9, 0.9, 0.9));
        auto groundMaterial = std::make_shared<rt::materials::Lambertian>(checkerTex);
        world.add(std::make_shared<rt::shapes::Sphere>(rt::Point3(0, -1000, 0), 1000, groundMaterial));

        // Generate random small spheres
        for (auto a = -11; a < 11; a++) {
            for (auto b = -11; b < 11; b++) {
                auto chooseMaterial = rt::randDouble();
                rt::Point3 center(a + 0.9 * rt::randDouble(), 0.2, b + 0.9 * rt::randDouble());

                if ((center - rt::Point3(4, 0.2, 0)).length() > 0.9) {
                    std::shared_ptr<rt::IMaterial> sphereMat;

                    if (chooseMaterial < 0.8) {
                        // diffuse
                        auto albedo = rt::color::rgb::random() * rt::color::rgb::random();
                        sphereMat = std::make_shared<rt::materials::Lambertian>(albedo);
                        auto centerEnd = center + rt::Vec3(0, rt::randDouble(0, 0.5), 0);
                        world.add(std::make_shared<rt::shapes::MovingSphere>(
                            center, centerEnd, 0.0, 1.0, 0.2, sphereMat));
                    }
                    else if (chooseMaterial < 0.95) {
                        // metal
                        auto albedo = rt::color::rgb::random(0.5, 1);
                        auto fuzz = rt::randDouble(0, 0.5);
                        sphereMat = std::make_shared<rt::materials::Metallic>(albedo, fuzz);
                        world.add(std::make_shared<rt::shapes::Sphere>(center, 0.2, sphereMat));
                    }
                    else {
                        // glass
                        sphereMat = std::make_shared<rt::materials::Dielectric>(1.5);
                        world.add(std::make_shared<rt::shapes::Sphere>(center, 0.2, sphereMat));
                    }
                }
            }
        }

        // Generate non-random big spheres
        auto mat1 = std::make_shared<rt::materials::Dielectric>(1.5);
        world.add(std::make_shared<rt::shapes::Sphere>(rt::Point3(0, 1, 0), 1.0, mat1));

        auto mat2 = std::make_shared<rt::materials::Lambertian>(rt::color::rgb(0.4, 0.2, 0.1));
        world.add(std::make_shared<rt::shapes::Sphere>(rt::Point3(-4, 1, 0), 1.0, mat2));

        auto mat3 = std::make_shared<rt::materials::Metallic>(rt::color::rgb(0.7, 0.6, 0.5), 0.0);
        world.add(std::make_shared<rt::shapes::Sphere>(rt::Point3(4, 1, 0), 1.0, mat3));

        // Setup camera
        rt::Point3 lookFrom(13, 2, 3);
        rt::Point3 lookAt(0, 0, 0);
        rt::Vec3 viewUp(0, 1, 0);
        auto focusDistance = 10.0;
        auto aperture = 0.1;
        camera = rt::Camera(
            lookFrom, lookAt, viewUp, aspectRatio, 20, aperture, focusDistance, timeStart, timeEnd);
    }
};

class TwoSpheres : public Scene {
public:
    TwoSpheres(double aspectRatio, double timeStart = 0.0, double timeEnd = 0.0) {
        auto tex = std::make_shared<rt::textures::Checkered>(
            rt::color::rgb(0.2, 0.3, 0.1),
            rt::color::rgb(0.9, 0.9, 0.9));
        auto material = std::make_shared<rt::materials::Lambertian>(tex);
        world.add(std::make_shared<rt::shapes::Sphere>(rt::Point3(0, -10, 0), 10, material));
        world.add(std::make_shared<rt::shapes::Sphere>(rt::Point3(0,  10, 0), 10, material));

        // Setup camera
        rt::Point3 lookFrom(13.0, 2.0, 3.0);
        rt::Point3 lookAt(0.0, 0.0, 0.0);
        rt::Vec3 viewUp(0.0, 1.0, 0.0);
        auto focusDistance = 10.0;
        auto aperture = 0.1;
        auto vfov = 20.0;
        camera = rt::Camera(lookFrom, lookAt, viewUp, aspectRatio, vfov, aperture, focusDistance, 0.0, 1.0);
    }
};

class TwoPerlinSpheres : public Scene {
public:
    TwoPerlinSpheres(double aspectRatio, double timeStart = 0.0, double timeEnd = 0.0) {
        auto tex = std::make_shared<rt::textures::Noise>(4.0);

        world.add(std::make_shared<rt::shapes::Sphere>(
            rt::Point3(0, -1000, 0), 1000,
            std::make_shared<rt::materials::Lambertian>(tex)));

        world.add(std::make_shared<rt::shapes::Sphere>(
            rt::Point3(0, 2, 0), 2,
            std::make_shared<rt::materials::Lambertian>(tex)));

        // Setup camera
        rt::Point3 lookFrom(13.0, 2.0, 3.0);
        rt::Point3 lookAt(0.0, 0.0, 0.0);
        rt::Vec3 viewUp(0.0, 1.0, 0.0);
        auto focusDistance = 10.0;
        auto aperture = 0.1;
        auto vfov = 20.0;
        camera = rt::Camera(lookFrom, lookAt, viewUp, aspectRatio, vfov, aperture, focusDistance, 0.0, 1.0);
    }
};

using SceneConstructor = std::function<Scene(double, double, double)>;

std::unordered_map<unsigned, SceneConstructor> SceneSelector = {
    {0, [](double ar, double t0, double t1) { return RandomScene(ar, t0, t1); } },
    {1, [](double ar, double t0, double t1) { return TwoSpheres(ar, t0, t1); } },
    {2, [](double ar, double t0, double t1) { return TwoPerlinSpheres(ar, t0, t1); } }
};
