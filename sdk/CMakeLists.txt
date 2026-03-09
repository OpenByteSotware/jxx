
cmake_minimum_required(VERSION 3.16)
project(jxxsdk LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_POSITION_INDEPENDENT_CODE ON)

# Collect sources (.cpp) and headers (.h)
file(GLOB JXX_HEADERS CONFIGURE_DEPENDS
    "*.h"
)
file(GLOB JXX_SOURCES CONFIGURE_DEPENDS
    "*.cpp"
)

add_library(jxxsdk ${JXX_SOURCES} ${JXX_HEADERS})
set_target_properties(jxxsdk PROPERTIES LINKER_LANGUAGE CXX)

target_include_directories(jxxsdk
    PRIVATE
		${CMAKE_SOURCE_DIR}/lang
		${CMAKE_SOURCE_DIR}/util
		${CMAKE_SOURCE_DIR}/io
		${CMAKE_SOURCE_DIR}/net		
)

if(WIN32)
    # Add the definition for all targets
    add_compile_definitions(_HAS_STD_BYTE=0)
endif()

# Public include is current source dir because headers live next to sources
# If you prefer a nested include dir, we can reorganize later.
target_include_directories(jxxsdk PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

# On MSVC, silence some warnings for interface-heavy code
if(MSVC)
    target_compile_options(jxxsdk PRIVATE /permissive- /W4)
else()
    target_compile_options(jxxsdk PRIVATE -Wall -Wextra -Wpedantic)
endif()
