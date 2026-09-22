cmake_minimum_required(VERSION 3.16)
project(jxx)

set(CMAKE_CXX_STANDARD 17)

# Add subdirectories
add_subdirectory(sdk)
add_subdirectory(sdkgui)
add_subdirectory(gui_unit_test)
add_subdirectory(unit_test)
add_subdirectory(bench)
add_subdirectory(apps)
add_subdirectory(java_converter/outcpp)
add_subdirectory(3rdparty/gsl)
add_subdirectory(3rdparty/fmt)
add_subdirectory(3rdparty/libunicode)
add_subdirectory(3rdparty/pugixml)

# wxwidgets has to build on its own not inside this one, see readme for how to build
