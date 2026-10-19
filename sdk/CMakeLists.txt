cmake_minimum_required(VERSION 3.16)
project(jxxsdk LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_POSITION_INDEPENDENT_CODE ON)

if(NOT DEFINED JXX_HEADLESS)
    set(JXX_HEADLESS ON)
endif()

configure_file(
    "${CMAKE_CURRENT_SOURCE_DIR}/jxx.BuildConfig.h.in"
    "${CMAKE_CURRENT_BINARY_DIR}/generated/jxx.BuildConfig.h"
    @ONLY
)

set(ZLIB_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
set(ZLIB_BUILD_TESTING OFF CACHE BOOL "" FORCE)
set(ZLIB_BUILD_SHARED OFF CACHE BOOL "" FORCE)

option(JXX_WITH_LIBUNICODE "Enable libunicode backend" ON)

file(GLOB_RECURSE SRC_FILES CONFIGURE_DEPENDS
    "${CMAKE_CURRENT_SOURCE_DIR}/*.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/*.h"
)

add_library(jxxsdk ${SRC_FILES})

set_target_properties(jxxsdk PROPERTIES
    LINKER_LANGUAGE CXX
)

target_compile_features(jxxsdk PUBLIC cxx_std_17)

target_compile_definitions(jxxsdk
    PUBLIC
        JXX_HEADLESS=$<BOOL:${JXX_HEADLESS}>
)

target_include_directories(jxxsdk
    PRIVATE
        "${CMAKE_CURRENT_SOURCE_DIR}/defines"
        "${CMAKE_CURRENT_SOURCE_DIR}/lang"
        "${CMAKE_CURRENT_SOURCE_DIR}/util"
        "${CMAKE_CURRENT_SOURCE_DIR}/io"
        "${CMAKE_CURRENT_SOURCE_DIR}/math"
        "${CMAKE_CURRENT_SOURCE_DIR}/net"
        "${CMAKE_CURRENT_SOURCE_DIR}/nio"
        "${CMAKE_CURRENT_SOURCE_DIR}/security"
        "${CMAKE_CURRENT_SOURCE_DIR}/org"
        "${CMAKE_CURRENT_SOURCE_DIR}/com"
        "${CMAKE_CURRENT_SOURCE_DIR}/ext"
    PUBLIC
        "${CMAKE_CURRENT_SOURCE_DIR}"
        "${CMAKE_CURRENT_BINARY_DIR}/generated"
        "${CMAKE_CURRENT_SOURCE_DIR}/../3rdparty/gsl/include"
        "${CMAKE_CURRENT_SOURCE_DIR}/../3rdparty/fmt/include"
        "${CMAKE_CURRENT_SOURCE_DIR}/../3rdparty/libunicode/src"
        "${CMAKE_CURRENT_SOURCE_DIR}/../3rdparty/pugixml/src"
        "${CMAKE_CURRENT_SOURCE_DIR}/../3rdparty/openssl/include"
        "${CMAKE_CURRENT_SOURCE_DIR}/../3rdparty/zlib"
        "${CMAKE_CURRENT_SOURCE_DIR}/../3rdparty/jpegturbo"
		)

if(MSVC)
    target_compile_definitions(jxxsdk
        PRIVATE
            _HAS_STD_BYTE=0
            WIN32_LEAN_AND_MEAN
            NOMINMAX
    )

    target_compile_options(jxxsdk
        PRIVATE
            /utf-8
            /permissive-
            /W4
            /MP
    )
elseif(CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang|AppleClang")
    target_compile_options(jxxsdk
        PRIVATE
            -Wall
            -Wextra
            -Wpedantic
    )
else()
    message(WARNING
        "jxxsdk: unrecognized C++ compiler '${CMAKE_CXX_COMPILER_ID}'. "
        "No compiler-specific warning options were added."
    )
endif()

if(JXX_HEADLESS)
    file(GLOB_RECURSE JXX_HEADLESS_CHECK_FILES CONFIGURE_DEPENDS
        "${CMAKE_CURRENT_SOURCE_DIR}/*.h"
        "${CMAKE_CURRENT_SOURCE_DIR}/*.cpp"
    )
    foreach(_jxx_source IN LISTS JXX_HEADLESS_CHECK_FILES)
        file(READ "${_jxx_source}" _jxx_source_text)
        if(_jxx_source_text MATCHES "#[ \t]*include[ \t]*[<\"]wx/")
            message(FATAL_ERROR
                "Headless SDK source includes wxWidgets: ${_jxx_source}")
        endif()
    endforeach()
endif()
