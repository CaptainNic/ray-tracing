#include <fstream>
#include <iostream>
#include <string>

void usage() {
    std::cerr
        << std::endl
        << "Usage: hello-ppm.exe <width> <height> <outFile>" << std::endl
        << std::endl;
}

int main(int argc, char** argv)
{
    if (argc != 4) {
        std::cerr << "Expected 3 args, got " << argc - 1 << std::endl;
        usage();
        return -1;
    }

    size_t image_width = 0;
    size_t image_height = 0;
    try {
        image_width = std::stoi(argv[1]);
        image_height = std::stoi(argv[2]);
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

    // Write header
    outFile << "P3" << std::endl
        << image_width << ' ' << image_height << std::endl
        << "255" << std::endl;

    // Write contents
    for (int yIdx = image_height - 1; yIdx >= 0; --yIdx) {
        std::cout << "\rScanlines remaining: " << yIdx << ' ' << std::flush;
        for (int xIdx = 0; xIdx < image_width; ++xIdx) {
            auto r = double(xIdx) / (image_width - 1);
            auto g = double(yIdx) / (image_height - 1);
            auto b = 0.25;

            int ir = static_cast<int>(255.999 * r);
            int ig = static_cast<int>(255.999 * g);
            int ib = static_cast<int>(255.999 * b);
        
            outFile << ir << ' ' << ig << ' ' << ib << std::endl;
        }
    }

    std::cout << std::endl << "Done" << std::endl;
}