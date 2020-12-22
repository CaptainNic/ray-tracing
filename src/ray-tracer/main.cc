#include <fstream>
#include <iostream>
#include <string>
#include "Camera.h"
#include "color.h"
#include "HitList.h"
#include "rtmath.h"
#include "shapes/sphere.h"

using rtmath::Point3;
using rtmath::Vec3;
using rtmath::Ray;

void usage() {
    std::cerr
        << std::endl
        << "Usage: ray-tracer.exe <width> <height> <outFile>" << std::endl
        << std::endl;
}

color::rgb ray_color(const rtmath::Ray& r, const rtmath::IHittable& world, unsigned depth) {
    rtmath::HitRecord rec;
    if (world.hit(r, 0.001, rtmath::infinity, rec)) {
        // Hard-coded diffuse using random unit sphere approximation, bouncing `depth` times.
        auto target = rec.p + rec.normal + rtmath::randVecInUnitSphere();
        return 0.5 * ray_color(Ray(rec.p, target - rec.p), world, depth - 1);

        // Normal map to RGB
        //return 0.5 * (rec.normal + color::rgb(1, 1, 1));

        // Paint it red
        //return color::rgb(1, 0, 0);
    }

    // Linear blend on y from white -> purpleish.
    auto unit_direction = rtmath::unit_vector(r.direction());
    auto t = 0.5 * (unit_direction.y() + 1.0);
    return (1.0 - t) * color::rgb(1.0, 1.0, 1.0) + t * color::rgb(0.5, 0.1, 1.0);
}

int main(int argc, char** argv)
{
    // Todo: add a dials for these.
    const unsigned samplesPerPx = 100;
    const unsigned maxDepth = 50;

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
    rtmath::HitList world;
    world.add(std::make_shared<rtshapes::Sphere>(Point3(0.0,    0.0, -1.0),   0.5));
    world.add(std::make_shared<rtshapes::Sphere>(Point3(0.0, -100.5, -1.0), 100.0));

    Camera camera(rtmath::Point3(0, 0, 0), imageWidth, imageHeight);

    /***** Render *****/

    // Write header
    outFile << "P3" << std::endl
        << imageWidth << ' ' << imageHeight << std::endl
        << "255" << std::endl;

    // Write contents
    for (int yIdx = imageHeight - 1; yIdx >= 0; --yIdx) {
        std::cout << "\rScanlines remaining: " << yIdx << ' ' << std::flush;
        for (int xIdx = 0; xIdx < imageWidth; ++xIdx) {
            color::rgb px_color(0, 0, 0);
            for (int sample = 0; sample < samplesPerPx; ++sample) {
                auto u = (xIdx + rtmath::randDouble()) / (imageWidth - 1);
                auto v = (yIdx + rtmath::randDouble()) / (imageHeight - 1);
                auto r = camera.getRay(u, v);
                px_color += ray_color(r, world, maxDepth);
            }

            color::write(outFile, px_color, samplesPerPx);
        }
    }

    std::cout << std::endl << "Done" << std::endl;
}