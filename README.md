# Ray Tracing in One Weekend
Following along w/ https://raytracing.github.io/books/RayTracingInOneWeekend.html

## Prerequisites
CMake 3.17+

## Setup
```
mkdir build
cd build
cmake ..
cmake --build .
```

## Running
Run the tester app:
```
tester.exe <width> <height> <scene#> <outPath>
```

Example: `tester.exe 800 600 2 image.ppm` will render scene 2 (perlin noise test) at 800x600 to the file `image.ppm`.