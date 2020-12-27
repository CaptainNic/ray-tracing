add_executable(tester
    main.cc
)

target_include_directories(tester
    PUBLIC
    ../ray-tracer
)

target_link_libraries(tester ray-tracer)