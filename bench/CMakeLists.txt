cmake_minimum_required(VERSION 3.16)

option(
    JXX_BENCH_FETCH_GOOGLE_BENCHMARK
    "Fetch Google Benchmark when an installed package is unavailable"
    ON
)

find_package(benchmark CONFIG QUIET)

if(NOT benchmark_FOUND)
    if(NOT JXX_BENCH_FETCH_GOOGLE_BENCHMARK)
        message(FATAL_ERROR
            "Google Benchmark was not found. Install benchmark with a CMake "
            "package configuration, provide benchmark_DIR/CMAKE_PREFIX_PATH, "
            "or configure with -DJXX_BENCH_FETCH_GOOGLE_BENCHMARK=ON."
        )
    endif()

    include(FetchContent)

    # Do not build Google Benchmark's own test suite or installation targets.
    # These options do not disable the JXX benchmark executable.
    set(BENCHMARK_ENABLE_TESTING ON CACHE BOOL "" FORCE)
    set(BENCHMARK_ENABLE_GTEST_TESTS ON CACHE BOOL "" FORCE)
    set(BENCHMARK_ENABLE_INSTALL ON CACHE BOOL "" FORCE)

    FetchContent_Declare(
        googlebenchmark
        GIT_REPOSITORY https://github.com/google/benchmark.git
        GIT_TAG v1.9.4
        GIT_SHALLOW TRUE
    )

    FetchContent_MakeAvailable(googlebenchmark)
endif()

# Automatically discover benchmark sources under:
#   bench/lang/
#   bench/util/
#   bench/io/
#   bench/util/concurrent/
# and any future package subdirectory.
file(
    GLOB_RECURSE
    JXX_BENCH_SOURCES
    CONFIGURE_DEPENDS
    "${CMAKE_CURRENT_SOURCE_DIR}/jxx_*_bench.cpp"
)

if(NOT JXX_BENCH_SOURCES)
    message(FATAL_ERROR
        "No JXX benchmark sources matching jxx_*_bench.cpp were found under "
        "${CMAKE_CURRENT_SOURCE_DIR}."
    )
endif()

add_executable(
    jxx_bench
    ${JXX_BENCH_SOURCES}
)

target_compile_features(
    jxx_bench
    PRIVATE
        cxx_std_17
)

target_link_libraries(
    jxx_bench
    PRIVATE
        jxxsdk
        benchmark::benchmark
        benchmark::benchmark_main
)

if(MSVC)
    target_compile_options(
        jxx_bench
        PRIVATE
            /utf-8
            /permissive-
            /W4
            /bigobj
    )
elseif(CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang|AppleClang")
    target_compile_options(
        jxx_bench
        PRIVATE
            -Wall
            -Wextra
            -Wpedantic
    )
else()
    message(WARNING
        "JXX benchmarks: unrecognized C++ compiler "
        "'${CMAKE_CXX_COMPILER_ID}'. "
        "No compiler-specific warning options were added."
    )
endif()

set_target_properties(
    jxx_bench
    PROPERTIES
        RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bench"
)

# Preserve the package-directory layout in Visual Studio.
source_group(
    TREE "${CMAKE_CURRENT_SOURCE_DIR}"
    PREFIX "Benchmark Sources"
    FILES ${JXX_BENCH_SOURCES}
)

message(STATUS "JXX benchmark sources:")
foreach(JXX_BENCH_SOURCE IN LISTS JXX_BENCH_SOURCES)
    file(
        RELATIVE_PATH
        JXX_BENCH_RELATIVE_SOURCE
        "${CMAKE_CURRENT_SOURCE_DIR}"
        "${JXX_BENCH_SOURCE}"
    )
    message(STATUS "  ${JXX_BENCH_RELATIVE_SOURCE}")
endforeach()