cmake_minimum_required(VERSION 3.16)
project(jxx_benchmarks LANGUAGES CXX)

# Require C++17 or later
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
enable_testing()

include(FetchContent)
FetchContent_Declare(
  benchmark
  URL https://github.com/google/benchmark/archive/192ef10025eb2c4cdd392bc502f0c852196baa48.zip
)
FetchContent_MakeAvailable(benchmark)

# Detect if building on Windows with Visual Studio
if (WIN32 AND MSVC)
    message(STATUS "Building on Windows with Visual Studio")
    add_definitions(-DNOMINMAX)   # Avoid min/max macro conflicts
    add_compile_options(/MP)      # Enable multi-core compilation
endif()

# Fetch Google Benchmark
#include(FetchContent)
#FetchContent_Declare(
#    benchmark
#    GIT_REPOSITORY https://github.com/google/benchmark.git
#    GIT_TAG v1.8.3
#)
#FetchContent_MakeAvailable(benchmark)

# Find all benchmark source files
file(GLOB_RECURSE BENCH_SOURCES CONFIGURE_DEPENDS
    "${CMAKE_CURRENT_SOURCE_DIR}/jxx_test_bench_*.cpp"
)

if (BENCH_SOURCES STREQUAL "")
    message(WARNING "No benchmark source files found (jxx_test_bench_*.cpp)")
else()
    message(STATUS "Found benchmark files: ${BENCH_SOURCES}")
endif()

if(WIN32)
    # Add the definition for all targets
    add_compile_definitions(_HAS_STD_BYTE=0)
endif()


# Create one executable per benchmark file
foreach(bench_file ${BENCH_SOURCES})
    get_filename_component(bench_name ${bench_file} NAME_WE) # name without extension
    set(target_name "${bench_name}_bench")

    add_executable(${target_name} ${bench_file})

    target_link_libraries(${target_name}
        PRIVATE
            benchmark::benchmark
            benchmark::benchmark_main
    )

    if(UNIX AND NOT APPLE)
    target_link_libraries(${target_name} PRIVATE dl)
    endif()

    # Always link pthread on Linux
    if (UNIX)
        find_package(Threads REQUIRED)
        target_link_libraries(${target_name} PRIVATE Threads::Threads)
    endif()

    # Optional: set output directory
    set_target_properties(${target_name} PROPERTIES
        RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin"
    )
endforeach()

target_include_directories(benchmark
    PRIVATE
		${CMAKE_SOURCE_DIR}/sdk
		${CMAKE_SOURCE_DIR}/sdk/defines
		${CMAKE_SOURCE_DIR}/sdk/lang
		${CMAKE_SOURCE_DIR}/sdk/util
		${CMAKE_SOURCE_DIR}/sdk/io
		${CMAKE_SOURCE_DIR}/sdk/net		
)
