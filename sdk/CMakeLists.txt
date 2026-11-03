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

# Keep the JXX SDK itself static on both Windows and Linux. Its PUBLIC link
# interface propagates the native libraries needed by final executables.
add_library(jxxsdk STATIC ${SRC_FILES})

set_target_properties(jxxsdk PROPERTIES
    LINKER_LANGUAGE CXX
)

target_compile_features(jxxsdk PUBLIC cxx_std_17)

target_compile_definitions(jxxsdk
    PUBLIC
        JXX_HEADLESS=$<BOOL:${JXX_HEADLESS}>
)

# OpenSSL integration:
# - Windows uses the vendored DLLs and their import libraries.
# - Linux and other Unix-like targets use the installed shared OpenSSL package.
if(WIN32)
    set(JXX_OPENSSL_ROOT
        "${CMAKE_CURRENT_SOURCE_DIR}/../3rdparty/openssl"
        CACHE PATH
        "JXX vendored OpenSSL root"
    )

    set(JXX_OPENSSL_INCLUDE_DIR "${JXX_OPENSSL_ROOT}/include")
    set(JXX_OPENSSL_CRYPTO_IMPLIB "${JXX_OPENSSL_ROOT}/libcrypto.lib")
    set(JXX_OPENSSL_CRYPTO_DLL "${JXX_OPENSSL_ROOT}/libcrypto-1_1-x64.dll")
    set(JXX_OPENSSL_SSL_IMPLIB "${JXX_OPENSSL_ROOT}/libssl.lib")
    set(JXX_OPENSSL_SSL_DLL "${JXX_OPENSSL_ROOT}/libssl-1_1-x64.dll")

    foreach(_jxx_openssl_path IN ITEMS
            "${JXX_OPENSSL_INCLUDE_DIR}"
            "${JXX_OPENSSL_CRYPTO_IMPLIB}"
            "${JXX_OPENSSL_CRYPTO_DLL}"
            "${JXX_OPENSSL_SSL_IMPLIB}"
            "${JXX_OPENSSL_SSL_DLL}")
        if(NOT EXISTS "${_jxx_openssl_path}")
            message(FATAL_ERROR
                "Required vendored OpenSSL path does not exist: "
                "${_jxx_openssl_path}"
            )
        endif()
    endforeach()

    if(NOT TARGET OpenSSL::Crypto)
        add_library(OpenSSL::Crypto SHARED IMPORTED GLOBAL)
        set_target_properties(OpenSSL::Crypto PROPERTIES
            IMPORTED_IMPLIB "${JXX_OPENSSL_CRYPTO_IMPLIB}"
            IMPORTED_LOCATION "${JXX_OPENSSL_CRYPTO_DLL}"
            INTERFACE_INCLUDE_DIRECTORIES "${JXX_OPENSSL_INCLUDE_DIR}"
        )
    endif()

    if(NOT TARGET OpenSSL::SSL)
        add_library(OpenSSL::SSL SHARED IMPORTED GLOBAL)
        set_target_properties(OpenSSL::SSL PROPERTIES
            IMPORTED_IMPLIB "${JXX_OPENSSL_SSL_IMPLIB}"
            IMPORTED_LOCATION "${JXX_OPENSSL_SSL_DLL}"
            INTERFACE_INCLUDE_DIRECTORIES "${JXX_OPENSSL_INCLUDE_DIR}"
            INTERFACE_LINK_LIBRARIES "OpenSSL::Crypto"
        )
    endif()
else()
    set(OPENSSL_USE_STATIC_LIBS FALSE)
    find_package(OpenSSL REQUIRED COMPONENTS SSL Crypto)
endif()

# libjpeg-turbo installation prefix. Expected layout:
#   <prefix>/include/turbojpeg.h
#   <prefix>/lib/turbojpeg.lib  (Windows)
# or the corresponding library on Linux.
set(JXX_TURBOJPEG_ROOT
    ""
    CACHE PATH
    "Installed libjpeg-turbo prefix"
)

find_path(JXX_TURBOJPEG_INCLUDE_DIR
    NAMES
        turbojpeg.h
    HINTS
        "${JXX_TURBOJPEG_ROOT}"
    PATHS
        ${CMAKE_PREFIX_PATH}
    PATH_SUFFIXES
        include
)

find_library(JXX_TURBOJPEG_LIBRARY
    NAMES
        turbojpeg
        turbojpeg-static
    HINTS
        "${JXX_TURBOJPEG_ROOT}"
    PATHS
        ${CMAKE_PREFIX_PATH}
    PATH_SUFFIXES
        lib
        lib64
)

include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(JXXTurboJPEG
    REQUIRED_VARS
        JXX_TURBOJPEG_INCLUDE_DIR
        JXX_TURBOJPEG_LIBRARY
)

if(NOT TARGET JXX::TurboJPEG)
    add_library(JXX::TurboJPEG UNKNOWN IMPORTED)
    set_target_properties(JXX::TurboJPEG PROPERTIES
        IMPORTED_LOCATION "${JXX_TURBOJPEG_LIBRARY}"
        INTERFACE_INCLUDE_DIRECTORIES "${JXX_TURBOJPEG_INCLUDE_DIR}"
    )
endif()

# These targets are expected to be created by the JXX root CMake project before
# sdk/ is added. Fail during configuration rather than at final executable link.
set(JXX_REQUIRED_DEPENDENCY_TARGETS
    fmt::fmt
    Microsoft.GSL::GSL
    unicode::unicode
    pugixml::pugixml
    zlibstatic
    OpenSSL::SSL
    OpenSSL::Crypto
    JXX::TurboJPEG
)

foreach(_jxx_dependency IN LISTS JXX_REQUIRED_DEPENDENCY_TARGETS)
    if(NOT TARGET ${_jxx_dependency})
        message(FATAL_ERROR
            "Required JXX dependency target is unavailable: ${_jxx_dependency}"
        )
    endif()
endforeach()

# PUBLIC is intentional. jxxsdk is static, so final executables must receive
# the complete third-party link interface.
target_link_libraries(jxxsdk
    PUBLIC
        ${JXX_REQUIRED_DEPENDENCY_TARGETS}
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
)

message(STATUS "JXX TurboJPEG include: ${JXX_TURBOJPEG_INCLUDE_DIR}")
message(STATUS "JXX TurboJPEG library: ${JXX_TURBOJPEG_LIBRARY}")
if(WIN32)
    message(STATUS "JXX OpenSSL crypto import library: ${JXX_OPENSSL_CRYPTO_IMPLIB}")
    message(STATUS "JXX OpenSSL SSL import library: ${JXX_OPENSSL_SSL_IMPLIB}")
else()
    message(STATUS "JXX OpenSSL crypto library: ${OPENSSL_CRYPTO_LIBRARY}")
    message(STATUS "JXX OpenSSL SSL library: ${OPENSSL_SSL_LIBRARY}")
endif()

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
