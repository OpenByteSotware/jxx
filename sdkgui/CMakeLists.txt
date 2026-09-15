cmake_minimum_required(VERSION 3.16)

project(jxx_gui LANGUAGES CXX)

#
# C++17 is a hard requirement for JXX GUI.
#
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

#
# Required for static/shared library reuse.
#
set(CMAKE_POSITION_INDEPENDENT_CODE ON)

#
# Optional build switch.
#
option(JXX_BUILD_GUI "Build JXX GUI Library" ON)

if(NOT JXX_BUILD_GUI)
    return()
endif()

#
# Find all source files.
#
file(GLOB_RECURSE SRC_FILES CONFIGURE_DEPENDS
    "${CMAKE_CURRENT_SOURCE_DIR}/*.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/*.h"
)

#
# GUI runtime library.
#
add_library(jxx_gui ${SRC_FILES})

set_target_properties(
    jxx_gui
    PROPERTIES
        LINKER_LANGUAGE CXX
)

#
# Core JXX dependency.
#
target_link_libraries(
    jxx_gui
    PUBLIC
        jxxsdk
)

#
# wxWidgets dependency.
#
find_package(wxWidgets REQUIRED)

include(${wxWidgets_USE_FILE})

target_link_libraries(
    jxx_gui
    PUBLIC
        ${wxWidgets_LIBRARIES}
)

#
# Public include directories.
#
target_include_directories(
    jxx_gui
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}
)

#
# Access to JXX runtime headers.
# Adjust JXX_ROOT if desired.
#
target_include_directories(
    jxx_gui
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/../defines
        ${CMAKE_CURRENT_SOURCE_DIR}/../lang
        ${CMAKE_CURRENT_SOURCE_DIR}/../util
        ${CMAKE_CURRENT_SOURCE_DIR}/../io
        ${CMAKE_CURRENT_SOURCE_DIR}/../math
        ${CMAKE_CURRENT_SOURCE_DIR}/../net
        ${CMAKE_CURRENT_SOURCE_DIR}/../nio
        ${CMAKE_CURRENT_SOURCE_DIR}/../security
        ${CMAKE_CURRENT_SOURCE_DIR}/../org
        ${CMAKE_CURRENT_SOURCE_DIR}/../com
        ${CMAKE_CURRENT_SOURCE_DIR}/../ext
)

#
# Match JXX third-party include layout.
#
target_include_directories(
    jxx_gui
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/../3rdparty/gsl/include
        ${CMAKE_CURRENT_SOURCE_DIR}/../3rdparty/fmt/include
        ${CMAKE_CURRENT_SOURCE_DIR}/../3rdparty/libunicode/src
        ${CMAKE_CURRENT_SOURCE_DIR}/../3rdparty/pugixml/src
)

#
# Compile features.
#
target_compile_features(
    jxx_gui
    PRIVATE
        cxx_std_17
)

#
# Global definitions.
#
add_compile_definitions(_HAS_STD_BYTE=0)

option(JXX_WITH_LIBUNICODE
    "Enable libunicode backend"
    ON)

#
# Windows-specific configuration.
#
if(WIN32)

    add_compile_definitions(
        WIN32_LEAN_AND_MEAN
        _HAS_STD_BYTE=0
    )

    add_definitions(
        -DNOMINMAX
    )

    add_compile_options(
        /MP
    )

endif()

#
# Warning levels.
#
if(MSVC)

    target_compile_options(
        jxx_gui
        PRIVATE
            /utf-8
            /permissive-
            /W4
    )

else()

    target_compile_options(
        jxx_gui
        PRIVATE
            -Wall
            -Wextra
            -Wpedantic
    )

endif()