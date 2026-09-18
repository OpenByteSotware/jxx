cmake_minimum_required(VERSION 3.16)

project(
    guiunittests
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
    gui_all_tests
    "${CMAKE_CURRENT_SOURCE_DIR}/jxx_unit_test_main.cpp"
    ${TEST_SOURCES}
)

target_compile_features(
    gui_all_tests
    PRIVATE
        cxx_std_17
)

target_compile_definitions(
    gui_all_tests
    PRIVATE
        _HAS_STD_BYTE=0
)

if(WIN32 AND MSVC)
    target_compile_definitions(
        gui_all_tests
        PRIVATE
            WIN32_LEAN_AND_MEAN
            NOMINMAX
    )

    target_compile_options(
        gui_all_tests
        PRIVATE
            /MP
    )
endif()

set(JXX_SDK_DIR
    "${JXX_TOP_LEVEL_DIR}/sdk"
    CACHE PATH "Path to the core JXX SDK"
)

set(JXX_GUI_SDK_DIR
    "${JXX_TOP_LEVEL_DIR}/sdkgui"
    CACHE PATH "Path to the JXX GUI SDK"
)

set(JXX_WXWIDGETS_DIR
    "${JXX_TOP_LEVEL_DIR}/3rdparty/wxWidgets"
    CACHE PATH "Path to the bundled wxWidgets source tree"
)

target_include_directories(
    gui_all_tests
    PRIVATE
        "${JXX_GUI_SDK_DIR}"
        "${JXX_SDK_DIR}"
        "${JXX_SDK_DIR}/defines"
        "${JXX_SDK_DIR}/lang"
        "${JXX_SDK_DIR}/util"
        "${JXX_SDK_DIR}/io"
        "${JXX_SDK_DIR}/math"
        "${JXX_SDK_DIR}/net"
        "${JXX_SDK_DIR}/nio"
        "${JXX_SDK_DIR}/security"
        "${JXX_SDK_DIR}/org"
        "${JXX_SDK_DIR}/com"
        "${JXX_SDK_DIR}/ext"
)

# Match the wxWidgets configuration known to build successfully for this tree.
# These values must be set before add_subdirectory(wxWidgets).
set(wxUSE_STC OFF CACHE BOOL
    "Disable wxWidgets styled text control" FORCE)
set(wxUSE_STL ON CACHE BOOL
    "Use STL-backed wxWidgets classes" FORCE)
set(wxUSE_WEBVIEW OFF CACHE BOOL
    "Disable wxWidgets web view" FORCE)
set(wxBUILD_SHARED OFF CACHE BOOL
    "Build static wxWidgets libraries" FORCE)
set(wxBUILD_MONOLITHIC OFF CACHE BOOL
    "Build separate wxWidgets libraries" FORCE)
set(wxBUILD_COMPATIBILITY 3.1 CACHE STRING
    "wxWidgets backward compatibility level" FORCE)
set(wxBUILD_SAMPLES OFF CACHE STRING
    "Disable wxWidgets samples" FORCE)
set(wxBUILD_TESTS OFF CACHE STRING
    "Disable wxWidgets tests" FORCE)
set(wxBUILD_DEMOS OFF CACHE BOOL
    "Disable wxWidgets demos" FORCE)
set(wxUSE_GUI ON CACHE BOOL
    "Build wxWidgets GUI libraries" FORCE)

# Keep application and wxWidgets assertion configuration consistent.
set(wxBUILD_DEBUG_LEVEL 0 CACHE STRING
    "Disable wxWidgets assertion support" FORCE)

if(NOT TARGET wx::base OR NOT TARGET wx::core)
    add_subdirectory(
        "${JXX_WXWIDGETS_DIR}"
        "${CMAKE_BINARY_DIR}/3rdparty/wxWidgets"
        EXCLUDE_FROM_ALL
    )
endif()


target_link_libraries(
    gui_all_tests
    PRIVATE
        jxxsdk
		jxx_gui
        GTest::gtest
        GTest::gmock
        fmt::fmt
        Microsoft.GSL::GSL
        unicode::unicode
        pugixml::pugixml
		wx::core
        wx::base
)

if(UNIX AND NOT APPLE)
    target_link_libraries(
        gui_all_tests
        PRIVATE
            dl
    )
endif()

include(GoogleTest)

gtest_discover_tests(
    gui_all_tests
    DISCOVERY_MODE POST_BUILD
    DISCOVERY_TIMEOUT 60
)






