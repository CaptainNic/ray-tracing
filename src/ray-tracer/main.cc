#include <fstream>
#include <iostream>
#include <string>
#include "color.h"
#include "ray.h"
#include "vec3.h"

using rtmath::point3;
using rtmath::vec3;
using rtmath::ray;

void usage() {
    std::cerr
        << std::endl
        << "Usage: ray-tracer.exe <width> <height> <outFile>" << std::endl
        << std::endl;
}

bool hitSphere(const point3& center, double radius, const ray& r) {
    // ax^2 + bx + c = 0    OR    x = (-b +- sqrt(b^2-4ac))/2a
    // As per the quadratic equation:
    // The discriminant (b^2 - 4ac) must be non-negative for there to be
    // a solution since you can't get a real number by sqrt of a negative.
    // Additionally, if the discriminant is 0, the ray hits the sphere in
    // only one location, thus it is a tangent.
    // If the discriminant is positive, the ray intersects 2 points of the
    // sphere. We consider this a hit.
    auto oc = r.origin() - center;
    auto a = rtmath::dot(r.direction(), r.direction());
    auto b = 2.0 * rtmath::dot(oc, r.direction());
    auto c = rtmath::dot(oc, oc) - (radius * radius);
    auto discriminant = (b * b) - (4 * a * c);
    return (discriminant > 0);
}

color::rgb ray_color(const rtmath::ray& r) {
    // If we hit the sphere, paint it red.
    if (hitSphere(point3(0, 0, -1), 0.5, r)) {
        return color::rgb(1, 0, 0);
    }

    // Linear blend on y from white -> purpleish.
    auto unit_direction = rtmath::unit_vector(r.direction());
    auto t = 0.5 * (unit_direction.y() + 1.0);
    return (1.0 - t) * color::rgb(1.0, 1.0, 1.0) + t * color::rgb(0.5, 0.1, 1.0);
}

int main(int argc, char** argv)
{
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

    /***** Setup Camera *****/
    auto aspectRatio = imageWidth / imageHeight;
    auto viewportHeight = 2.0;
    auto viewportWidth = aspectRatio * viewportHeight;
    auto focalLength = 1.0;

    auto origin = rtmath::point3(0, 0, 0);
    auto horizontal = rtmath::vec3(viewportWidth, 0, 0);
    auto vertical = rtmath::vec3(0, viewportHeight, 0);
    auto lowerLeft = origin - (horizontal / 2) - (vertical / 2) - rtmath::vec3(0, 0, focalLength);

    /***** Setup Scene *****/
    // TBD...

    /***** Render *****/

    // Write header
    outFile << "P3" << std::endl
        << imageWidth << ' ' << imageHeight << std::endl
        << "255" << std::endl;

    // Write contents
    for (int yIdx = imageHeight - 1; yIdx >= 0; --yIdx) {
        std::cout << "\rScanlines remaining: " << yIdx << ' ' << std::flush;
        for (int xIdx = 0; xIdx < imageWidth; ++xIdx) {
            auto u = static_cast<double>(xIdx) / (imageWidth - 1);
            auto v = static_cast<double>(yIdx) / (imageHeight - 1);
            rtmath::ray r(origin, lowerLeft + (u * horizontal) + (v * vertical) - origin);
            auto px = ray_color(r);
            color::write(outFile, px);
        }
    }

    std::cout << std::endl << "Done" << std::endl;
}