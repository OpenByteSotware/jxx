cmake_minimum_required(VERSION 3.16)

if(JXX_HEADLESS)
    message(FATAL_ERROR "sdkgui must not be included when JXX_HEADLESS=ON")
endif()

if(NOT TARGET jxxsdk)
    message(FATAL_ERROR "sdkgui requires the canonical jxxsdk target")
endif()

# GUI sources augment jxxsdk so applications always link one JXX runtime and
# always call only jxx::initialize(). The canonical beans implementation lives
# in sdk/beans; sdkgui/beans intentionally contains no duplicate sources.
file(GLOB_RECURSE JXX_GUI_SOURCES CONFIGURE_DEPENDS
    "${CMAKE_CURRENT_SOURCE_DIR}/awt/*.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/awt/*.h"
    "${CMAKE_CURRENT_SOURCE_DIR}/gui/*.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/gui/*.h"
    "${CMAKE_CURRENT_SOURCE_DIR}/swing/*.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/swing/*.h"
    "${CMAKE_CURRENT_SOURCE_DIR}/jxx.gui.initialize.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/jxx.gui.initialize.h"
)

target_sources(jxxsdk PRIVATE ${JXX_GUI_SOURCES})

target_include_directories(jxxsdk
    PUBLIC
        "${CMAKE_CURRENT_SOURCE_DIR}"
)

# Support both CMake's FindwxWidgets module and configurations where the caller
# has already supplied wxWidgets_LIBRARIES and wxWidgets_INCLUDE_DIRS.
if(NOT wxWidgets_FOUND)
    find_package(wxWidgets REQUIRED COMPONENTS base core)
endif()

if(DEFINED wxWidgets_USE_FILE AND EXISTS "${wxWidgets_USE_FILE}")
    include("${wxWidgets_USE_FILE}")
endif()

if(wxWidgets_INCLUDE_DIRS)
    target_include_directories(jxxsdk PRIVATE ${wxWidgets_INCLUDE_DIRS})
endif()

if(TARGET wx::base)
    target_link_libraries(jxxsdk PUBLIC wx::base wx::core)
else()
    target_link_libraries(jxxsdk PUBLIC ${wxWidgets_LIBRARIES})
endif()

target_compile_definitions(jxxsdk
    PUBLIC
        JXX_HEADLESS=0
    PRIVATE
        wxDEBUG_LEVEL=0
)
