cmake_minimum_required(VERSION 3.17)

project(RayTracingInOneWeekend)

if (MSVC_VERSION GREATER_EQUAL "1900")
    include(CheckCXXCompilerFlag)
    CHECK_CXX_COMPILER_FLAG("/std:c++latest" _cpp_latest_flag_supported)
    if (_cpp_latest_flag_supported)
        add_compile_options("/std:c++latest")
    endif()
else()
    message(FATAL_ERROR "MSVC 1900 or higher is required for C++20 support.")
endif()

add_subdirectory(src)