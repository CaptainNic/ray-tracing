#include <chrono>
#include <fstream>
#include <iostream>
#include <string>
#include <thread>
#include <vector>

#include "Camera.h"
#include "color.h"
#include "HitList.h"
#include "rtmath.h"

#include "SceneSelector.h"

using rt::Point3;
using rt::Vec3;
using rt::Ray;

void usage() {
    std::cerr
        << std::endl
        << "Usage: ray-tracer.exe <width> <height> <scene#> <outFile>" << std::endl
        << std::endl;
}

rt::color::rgb ray_color(const rt::Ray& r, const rt::IHittable& world, unsigned depth) {
    rt::HitRecord rec;
    
    // If we've exceeded the ray bounce limit, no more light is gathered.
    if (depth <= 0) {
        return rt::color::rgb(0, 0, 0);
    }

    if (world.hit(r, 0.001, rt::infinity, rec)) {
        Ray scattered;
        rt::color::rgb attenuation;
        // Scatter light, bouncing up to `depth` times.
        if (rec.material->scatter(r, rec, attenuation, scattered)) {
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
    return (1.0 - t) * rt::color::rgb(1.0, 1.0, 1.0) + t * rt::color::rgb(0.5, 0.7, 1.0);
}

typedef std::vector<rt::color::rgb> RenderChunk;

void renderPixels(
    const unsigned imageWidth, const unsigned imageHeight,
    const unsigned samplesPerPx, const unsigned maxDepth,
    const rt::HitList& world, const rt::Camera& camera,
    std::shared_ptr<RenderChunk> pxMap,
    const unsigned yTop, const unsigned yBottom
) {
    // Render from top down
    auto yPos = yTop;
    auto xPos = 0;

    for (auto iter = pxMap->begin(); iter != pxMap->end(); ++iter) {
        for (auto sample = 0; sample < samplesPerPx; ++sample) {
            auto u = (xPos + rt::randDouble()) / (imageWidth - 1);
            auto v = (yPos + rt::randDouble()) / (imageHeight - 1);
            auto r = camera.getRay(u, v);
            *iter += ray_color(r, world, maxDepth);
        }

        // Increment pixel position
        ++xPos;
        if (xPos == imageWidth) {
            xPos = 0;
            --yPos;
        }
    }
}

int main(int argc, char** argv)
{
    // Todo: add a dials for these.
    const unsigned samplesPerPx = 100;
    const unsigned maxDepth = 50;
    const unsigned numThreads = 16;

    /***** Parse Input *****/
    if (argc != 5) {
        std::cerr << "Expected 4 args, got " << argc - 1 << std::endl;
        usage();
        return -1;
    }

    unsigned imageWidth = 0;
    unsigned imageHeight = 0;
    int sceneSelect = -1;
    try {
        imageWidth = std::stoi(argv[1]);
        imageHeight = std::stoi(argv[2]);
        if (imageWidth == 0 || imageHeight == 0) {
            throw std::range_error("Width and Height must each be greater than zero.");
        }

        sceneSelect = std::stoi(argv[3]);
        if (sceneSelect <= 0) {
            throw std::range_error("SceneSelect must be a number greater than zero.");
        }
    }
    catch (const std::exception& ex) {
        std::cerr << "Failed to parse arguments: " << ex.what() << std::endl;
        usage();
        return -1;
    }

    std::string filePath = argv[4];

    // Overwrite contents if exists
    auto outFile = std::ofstream(filePath, std::ios::out | std::ios::trunc);
    if (!outFile) {
        std::cerr << "Failed to open file for writing: " << filePath << std::endl;
        usage();
        return -1;
    }

    /***** Setup Scene *****/
    auto aspectRatio = static_cast<double>(imageWidth) / static_cast<double>(imageHeight);
    Scene scene = SceneSelector.at(sceneSelect)(aspectRatio, 0.0, 1.0);

    /***** Render *****/
    auto start = std::chrono::high_resolution_clock::now();
    
    // We clamp the thread count to imageHeight because we can't
    // split a single scan line over multiple threads at the moment.
    const unsigned threads = rt::clamp(numThreads, 1, imageHeight);

    // Round up to next full scan line so we don't skip rendering any lines due to uneven division.
    const unsigned linesPerThread = (static_cast<double>(imageHeight) / threads) + 1;
    auto threadPool = std::vector<std::thread>(threads);
    auto chunks = std::vector<std::shared_ptr<RenderChunk>>(threads);

    // Render each set of lines in a separate thread, starting from the top.
    for (auto i = 0; i < threads; ++i) {
        auto chunkNum = threads - i - 1;
        auto yTop = (linesPerThread * (i + 1));
        if (yTop > imageHeight) {
            // The thread for the top chunk may get a bit less work.
            yTop = imageHeight;
        }
        auto yBottom = linesPerThread * i;
        auto numPx = (yTop - yBottom) * imageWidth;
        chunks[chunkNum] = std::make_shared<RenderChunk>(numPx);
        threadPool.emplace_back(std::thread(renderPixels, 
            imageWidth, imageHeight, samplesPerPx, maxDepth, scene.world, scene.camera, chunks[chunkNum], yTop, yBottom));
    }

    for (auto& thread : threadPool) {
        if (thread.joinable()) {
            thread.join();
        }
    }

    auto renderEnd = std::chrono::high_resolution_clock::now();
    std::cout << "Render Time: " << std::chrono::duration_cast<std::chrono::milliseconds>(renderEnd - start).count() << "ms" << std::endl;

    /***** Write to Disk *****/

    // Write header
    outFile << "P3\n" << imageWidth << ' ' << imageHeight << "\n255\n";

    // Write contents
    for (auto chunkIter = chunks.cbegin(); chunkIter != chunks.cend(); ++chunkIter) {
        for (auto pxIter = (*chunkIter)->cbegin(); pxIter != (*chunkIter)->cend(); ++pxIter) {
            // This is not a pure "write" function, it is also doing some post-processing before writing to disk.
            rt::color::write(outFile, *pxIter, samplesPerPx);
        }
    }

    auto end = std::chrono::high_resolution_clock::now();
    std::cout
        << "Write to Disk Time: " << std::chrono::duration_cast<std::chrono::milliseconds>(end - renderEnd).count() << "ms" << std::endl
        << "Total Time: " << std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count() << "ms" << std::endl
        << "Done" << std::endl;
}