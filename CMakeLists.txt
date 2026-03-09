cmake_minimum_required(VERSION 3.16)
project(jxx)

set(CMAKE_CXX_STANDARD 17)

# Add subdirectories
add_subdirectory(sdk)
add_subdirectory(unit_test)
add_subdirectory(bench)
