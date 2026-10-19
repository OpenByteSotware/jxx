cmake_minimum_required(VERSION 3.16)
project(jxx LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_POSITION_INDEPENDENT_CODE ON)

option(JXX_HEADLESS "Build core and peerless AWT/Swing without wxWidgets" ON)
option(JXX_BUILD_TESTS "Build JXX unit tests" ON)
option(JXX_BUILD_BENCHMARKS "Build JXX benchmarks" OFF)
option(JXX_BUILD_APPS "Build JXX applications" OFF)

message(STATUS "JXX_HEADLESS=${JXX_HEADLESS}")

find_package(libjpeg-turbo CONFIG REQUIRED)

# Third-party projects must be declared before JXX targets that consume them.
foreach(_jxx_thirdparty IN ITEMS gsl fmt libunicode pugixml zlib)
    if(EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/3rdparty/${_jxx_thirdparty}/CMakeLists.txt")
        add_subdirectory("3rdparty/${_jxx_thirdparty}")
    endif()
endforeach()

add_subdirectory(sdk)

# sdkgui augments the canonical jxxsdk target. It is never parsed in a
# headless build, which prevents wxWidgets sources and headers from leaking in.
if(NOT JXX_HEADLESS)
    add_subdirectory(sdkgui)
endif()

if(JXX_BUILD_TESTS)
    if(EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/unit_test/CMakeLists.txt")
        add_subdirectory(unit_test)
    endif()
    if(NOT JXX_HEADLESS AND EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/gui_unit_test/CMakeLists.txt")
        add_subdirectory(gui_unit_test)
    endif()
endif()

if(JXX_BUILD_BENCHMARKS AND EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/bench/CMakeLists.txt")
    add_subdirectory(bench)
endif()

if(JXX_BUILD_APPS AND EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/apps/CMakeLists.txt")
    add_subdirectory(apps)
endif()
