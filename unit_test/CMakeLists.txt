cmake_minimum_required(VERSION 3.16)

project(
    unittests
    LANGUAGES CXX
)

enable_testing()

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

include(FetchContent)

FetchContent_Declare(
    googletest
    URL
        https://github.com/google/googletest/archive/52eb8108c5bdec04579160ae17225d66034bd723.zip
)

if(WIN32 AND MSVC)
    set(
        gtest_force_shared_crt
        ON
        CACHE BOOL ""
        FORCE
    )
endif()

FetchContent_MakeAvailable(googletest)

file(
    GLOB_RECURSE TEST_SOURCES
    CONFIGURE_DEPENDS
    "${CMAKE_CURRENT_SOURCE_DIR}/jxx_*.cpp"
)

add_executable(
    all_tests
    "${CMAKE_CURRENT_SOURCE_DIR}/jxx_unit_test_main.cpp"
    ${TEST_SOURCES}
)

target_compile_features(
    all_tests
    PRIVATE
        cxx_std_17
)

target_compile_definitions(
    all_tests
    PRIVATE
        _HAS_STD_BYTE=0
)

if(WIN32 AND MSVC)
    target_compile_definitions(
        all_tests
        PRIVATE
            WIN32_LEAN_AND_MEAN
            NOMINMAX
    )

    target_compile_options(
        all_tests
        PRIVATE
            /MP
    )
endif()

target_include_directories(
    all_tests
    PRIVATE
        "${CMAKE_SOURCE_DIR}/sdk"
        "${CMAKE_SOURCE_DIR}/sdk/defines"
        "${CMAKE_SOURCE_DIR}/sdk/lang"
        "${CMAKE_SOURCE_DIR}/sdk/util"
        "${CMAKE_SOURCE_DIR}/sdk/io"
        "${CMAKE_SOURCE_DIR}/sdk/math"
        "${CMAKE_SOURCE_DIR}/sdk/net"
        "${CMAKE_SOURCE_DIR}/sdk/nio"
        "${CMAKE_SOURCE_DIR}/sdk/security"
)

target_link_libraries(
    all_tests
    PRIVATE
        jxxsdk
        GTest::gtest
        GTest::gmock
        fmt::fmt
        Microsoft.GSL::GSL
        unicode::unicode
)

if(UNIX AND NOT APPLE)
    target_link_libraries(
        all_tests
        PRIVATE
            dl
    )
endif()

include(GoogleTest)

gtest_discover_tests(
    all_tests
    DISCOVERY_MODE POST_BUILD
    DISCOVERY_TIMEOUT 60
)