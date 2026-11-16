cmake_minimum_required(VERSION 3.16)

option(JXX_BENCH_FETCH_GOOGLE_BENCHMARK
    "Fetch Google Benchmark when an installed package is unavailable" ON)

find_package(benchmark CONFIG QUIET)

if(NOT benchmark_FOUND)
    if(NOT JXX_BENCH_FETCH_GOOGLE_BENCHMARK)
        message(FATAL_ERROR
            "Google Benchmark was not found. Install benchmark with a CMake "
            "package configuration, provide benchmark_DIR/CMAKE_PREFIX_PATH, "
            "or configure with -DJXX_BENCH_FETCH_GOOGLE_BENCHMARK=ON.")
    endif()

    include(FetchContent)
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

add_subdirectory(lang)
