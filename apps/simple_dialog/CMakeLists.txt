cmake_minimum_required(VERSION 3.16)

project(jxx_frame_dialog_example LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Expected repository layout:
# <jxx_root>/
#   sdk/
#   sdkgui/
#   3rdparty/wxWidgets/
#   apps/simple_dialog/
#
# Resolve all dependencies directly relative to this CMakeLists.txt.
# No environment variable, top-level root variable, or cached path is used.
get_filename_component(
    JXX_SDK_DIR
    "${CMAKE_CURRENT_LIST_DIR}/../../sdk"
    ABSOLUTE
)

get_filename_component(
    JXX_GUI_SDK_DIR
    "${CMAKE_CURRENT_LIST_DIR}/../../sdkgui"
    ABSOLUTE
)

get_filename_component(
    JXX_WXWIDGETS_DIR
    "${CMAKE_CURRENT_LIST_DIR}/../../3rdparty/wxWidgets"
    ABSOLUTE
)

if(NOT EXISTS "${JXX_SDK_DIR}/CMakeLists.txt")
    message(FATAL_ERROR
        "The core JXX SDK source tree was not found.\n"
        "Expected: ${JXX_SDK_DIR}/CMakeLists.txt"
    )
endif()

if(NOT EXISTS "${JXX_GUI_SDK_DIR}/CMakeLists.txt")
    message(FATAL_ERROR
        "The JXX GUI SDK source tree was not found.\n"
        "Expected: ${JXX_GUI_SDK_DIR}/CMakeLists.txt"
    )
endif()

if(NOT EXISTS "${JXX_WXWIDGETS_DIR}/CMakeLists.txt")
    message(FATAL_ERROR
        "The bundled wxWidgets source tree was not found.\n"
        "Expected: ${JXX_WXWIDGETS_DIR}/CMakeLists.txt"
    )
endif()

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
        "${CMAKE_CURRENT_BINARY_DIR}/wxWidgets"
        EXCLUDE_FROM_ALL
    )
endif()

if(NOT TARGET wx::base)
    message(FATAL_ERROR
        "Adding wxWidgets did not create the wx::base target"
    )
endif()

if(NOT TARGET wx::core)
    message(FATAL_ERROR
        "Adding wxWidgets did not create the wx::core target"
    )
endif()

add_executable(
    jxx_frame_dialog_example
    jxx_frame_dialog_example.cpp
)

target_compile_features(
    jxx_frame_dialog_example
    PRIVATE
        cxx_std_17
)

target_compile_definitions(
    jxx_frame_dialog_example
    PRIVATE
        wxDEBUG_LEVEL=0
)

if(MSVC)
    target_compile_definitions(
        jxx_frame_dialog_example
        PRIVATE
            WIN32_LEAN_AND_MEAN
            NOMINMAX
    )
endif()

# Keep GUI headers before core headers because the current source trees contain
# overlapping package headers and the GUI versions must take precedence here.
target_include_directories(
    jxx_frame_dialog_example
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

target_link_libraries(
    jxx_frame_dialog_example
    PRIVATE
        jxx_gui
        jxxsdk
        unicode::unicode
        fmt::fmt
        wx::core
        wx::base
)

set_target_properties(
    jxx_frame_dialog_example
    PROPERTIES
        RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin"
)

message(STATUS "Simple dialog source: ${CMAKE_CURRENT_LIST_DIR}")
message(STATUS "Simple dialog core SDK: ${JXX_SDK_DIR}")
message(STATUS "Simple dialog GUI SDK: ${JXX_GUI_SDK_DIR}")
message(STATUS "Simple dialog wxWidgets source: ${JXX_WXWIDGETS_DIR}")
