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
    "${CMAKE_CURRENT_SOURCE_DIR}/lang/jxx_*.cpp"
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
        "${CMAKE_CURRENT_SOURCE_DIR}/support"
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

# -----------------------------------------------------------------------------
# Vendored OpenSSL 1.1.1w
#
# Use absolute imported-library locations so Visual Studio never receives only
# the bare filename "libssl_static.lib". The root may be overridden when
# configuring with -DJXX_OPENSSL_ROOT=<path>.
# -----------------------------------------------------------------------------
set(
    JXX_OPENSSL_ROOT
    "${CMAKE_SOURCE_DIR}/3rdparty/openssl"
    CACHE PATH
    "Path to the vendored OpenSSL 1.1.1w directory"
)

set(
    JXX_OPENSSL_INCLUDE_DIR
    "${JXX_OPENSSL_ROOT}/include"
)

if(WIN32)
    set(
        JXX_OPENSSL_SSL_LIBRARY
        "${JXX_OPENSSL_ROOT}/libssl_static.lib"
    )
    set(
        JXX_OPENSSL_CRYPTO_LIBRARY
        "${JXX_OPENSSL_ROOT}/libcrypto_static.lib"
    )
else()
    set(
        JXX_OPENSSL_SSL_LIBRARY
        "${JXX_OPENSSL_ROOT}/libssl.a"
    )
    set(
        JXX_OPENSSL_CRYPTO_LIBRARY
        "${JXX_OPENSSL_ROOT}/libcrypto.a"
    )
endif()

foreach(
    required_path
    IN ITEMS
        "${JXX_OPENSSL_INCLUDE_DIR}"
        "${JXX_OPENSSL_SSL_LIBRARY}"
        "${JXX_OPENSSL_CRYPTO_LIBRARY}"
)
    if(NOT EXISTS "${required_path}")
        message(
            FATAL_ERROR
            "Required vendored OpenSSL path does not exist: ${required_path}"
        )
    endif()
endforeach()

add_library(
    JXX_OpenSSL_SSL
    STATIC
    IMPORTED
    GLOBAL
)
set_target_properties(
    JXX_OpenSSL_SSL
    PROPERTIES
        IMPORTED_LOCATION
            "${JXX_OPENSSL_SSL_LIBRARY}"
        INTERFACE_INCLUDE_DIRECTORIES
            "${JXX_OPENSSL_INCLUDE_DIR}"
)
add_library(
    JXX::OpenSSL::SSL
    ALIAS
    JXX_OpenSSL_SSL
)

add_library(
    JXX_OpenSSL_Crypto
    STATIC
    IMPORTED
    GLOBAL
)
set_target_properties(
    JXX_OpenSSL_Crypto
    PROPERTIES
        IMPORTED_LOCATION
            "${JXX_OPENSSL_CRYPTO_LIBRARY}"
        INTERFACE_INCLUDE_DIRECTORIES
            "${JXX_OPENSSL_INCLUDE_DIR}"
)
add_library(
    JXX::OpenSSL::Crypto
    ALIAS
    JXX_OpenSSL_Crypto
)

# jxxsdk is a static library, so OpenSSL is PUBLIC to propagate both vendored
# archives to final executables such as all_tests.
target_link_libraries(
    jxxsdk
    PUBLIC
        JXX::OpenSSL::SSL
        JXX::OpenSSL::Crypto
)

target_compile_definitions(
    jxxsdk
    PUBLIC
        JXX_WITH_OPENSSL_TLS=1
        JXX_OPENSSL_VERSION_1_1_1W=1
)

if(WIN32)
    target_link_libraries(
        jxxsdk
        PUBLIC
            Crypt32
            Ws2_32
    )
elseif(UNIX AND NOT APPLE)
    find_package(Threads REQUIRED)
    target_link_libraries(
        jxxsdk
        PUBLIC
            Threads::Threads
            dl
    )
endif()

# Link the test executable after jxxsdk has acquired its PUBLIC OpenSSL usage
# requirements. Do not add bare libssl/libcrypto filenames here.
target_link_libraries(
    all_tests
    PRIVATE
        jxxsdk
        GTest::gtest
        GTest::gmock
        fmt::fmt
        Microsoft.GSL::GSL
        unicode::unicode
        pugixml::pugixml
)

include(GoogleTest)

gtest_discover_tests(
    all_tests
    DISCOVERY_MODE POST_BUILD
    DISCOVERY_TIMEOUT 60
)
