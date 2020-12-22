#include <cassert>
#include <chrono>
#include <fstream>
#include <iostream>
#include <string>

#include "Camera.h"
#include "color.h"
#include "HitList.h"
#include "rtmath.h"
#include "materials/Lambertian.h"
#include "materials/Metallic.h"
#include "shapes/sphere.h"

using rt::Point3;
using rt::Vec3;
using rt::Ray;

void usage() {
    std::cerr
        << std::endl
        << "Usage: ray-tracer.exe <width> <height> <outFile>" << std::endl
        << std::endl;
}

rt::color::rgb ray_color(const rt::Ray& r, const rt::IHittable& world, unsigned depth) {
    rt::HitRecord rec;
    if (world.hit(r, 0.001, rt::infinity, rec)) {
        Ray scattered;
        rt::color::rgb attenuation;

        // Scatter light, bouncing up to `depth` times.
        if (rec.material && rec.material->scatter(r, rec, attenuation, scattered)) {
            return attenuation * ray_color(scattered, world, depth - 1);
        }

        return rt::color::rgb(0, 0, 0);

        // Normal map to RGB
        //return 0.5 * (rec.normal + color::rgb(1, 1, 1));

        // Paint it red
        //return color::rgb(1, 0, 0);
    }

    // Linear blend on y from white -> purpleish.
    auto unit_direction = rt::unit_vector(r.direction());
    auto t = 0.5 * (unit_direction.y() + 1.0);
    return (1.0 - t) * rt::color::rgb(1.0, 1.0, 1.0) + t * rt::color::rgb(0.5, 0.1, 1.0);
}

int main(int argc, char** argv)
{
    // Todo: add a dials for these.
    const unsigned samplesPerPx = 10;
    const unsigned maxDepth = 5;

    /***** Parse Input *****/
    if (argc != 4) {
        std::cerr << "Expected 3 args, got " << argc - 1 << std::endl;
        usage();
        return -1;
    }

    double imageWidth = 0.0;
    double imageHeight = 0.0;
    try {
        imageWidth = std::stoi(argv[1]);
        imageHeight = std::stoi(argv[2]);
        if (imageWidth == 0 || imageHeight == 0) {
            throw std::range_error("Width and Height must each be greater than zero.");
        }
    }
    catch (const std::exception& ex) {
        std::cerr << "Failed to parse width and height: " << ex.what() << std::endl;
        usage();
        return -1;
    }

    std::string filePath = argv[3];

    // Overwrite contents if exists
    auto outFile = std::ofstream(filePath, std::ios::out | std::ios::trunc);
    if (!outFile) {
        std::cerr << "Failed to open file for writing: " << filePath << std::endl;
        usage();
        return -1;
    }

    /***** Setup Scene *****/
    rt::HitList world;

    auto material_ground = std::make_shared<rt::materials::Lambertian>(rt::color::rgb(0.8, 0.8, 0.0));
    auto material_center = std::make_shared<rt::materials::Lambertian>(rt::color::rgb(0.7, 0.3, 0.3));
    auto material_left = std::make_shared<rt::materials::Metallic>(rt::color::rgb(0.8, 0.8, 0.8));
    auto material_right = std::make_shared<rt::materials::Metallic>(rt::color::rgb(0.8, 0.6, 0.2));

    world.add(std::make_shared<rt::shapes::Sphere>(rt::Point3( 0.0, -100.5, -1.0), 100.0, material_ground));
    world.add(std::make_shared<rt::shapes::Sphere>(rt::Point3( 0.0,    0.0, -1.0),   0.5, material_center));
    world.add(std::make_shared<rt::shapes::Sphere>(rt::Point3(-1.0,    0.0, -1.0),   0.5, material_left));
    world.add(std::make_shared<rt::shapes::Sphere>(rt::Point3( 1.0,    0.0, -1.0),   0.5, material_right));

    Camera camera(rt::Point3(0, 0, 0), imageWidth, imageHeight);

    /***** Render *****/
    auto start = std::chrono::high_resolution_clock::now();
    
    // Write header
    outFile << "P3\n" << imageWidth << ' ' << imageHeight << "\n255\n";

    // Write contents
    for (int yIdx = imageHeight - 1; yIdx >= 0; --yIdx) {
        std::cout << "\rScanlines remaining: " << yIdx << ' ' << std::flush;
        for (int xIdx = 0; xIdx < imageWidth; ++xIdx) {
            rt::color::rgb px_color(0, 0, 0);
            for (int sample = 0; sample < samplesPerPx; ++sample) {
                auto u = (xIdx + rt::randDouble()) / (imageWidth - 1);
                auto v = (yIdx + rt::randDouble()) / (imageHeight - 1);
                auto r = camera.getRay(u, v);
                px_color += ray_color(r, world, maxDepth);
            }

            rt::color::write(outFile, px_color, samplesPerPx);
        }
    }

    std::cout << "\nDone\n";

    auto end = std::chrono::high_resolution_clock::now();
    std::cout << "Total Time: " << std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count() << "ms" << std::endl;
}