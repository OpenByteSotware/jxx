cmake_minimum_required(VERSION 3.16)

project(jxx_frame_dialog_example LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Set this to the directory containing the core SDK CMakeLists.txt.
set(JXX_SDK_DIR "${CMAKE_CURRENT_LIST_DIR}/../../sdk"
    CACHE PATH "Path to the core JXX SDK")

# Set this to the directory containing the GUI SDK CMakeLists.txt.
set(JXX_GUI_SDK_DIR "${CMAKE_CURRENT_LIST_DIR}/../../sdkgui"
    CACHE PATH "Path to the JXX GUI SDK")

add_executable(jxx_frame_dialog_example
    jxx_frame_dialog_example.cpp
)

target_compile_definitions(jxx_frame_dialog_example
    PRIVATE
        wxDEBUG_LEVEL=0
)

target_compile_features(jxx_frame_dialog_example PRIVATE cxx_std_17)

target_link_libraries(jxx_frame_dialog_example
    PRIVATE
        jxx_gui
        jxxsdk
		unicode::unicode
		wx::core
		wx::base
)

# The current jxx_gui target publishes its SDK include directories as PRIVATE,
# so the application must include the public SDK roots explicitly.
target_include_directories(jxx_frame_dialog_example
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

if(MSVC)
    target_compile_definitions(jxx_frame_dialog_example PRIVATE
        _HAS_STD_BYTE=0
    )
endif()

set_target_properties(jxx_frame_dialog_example PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin"
)
