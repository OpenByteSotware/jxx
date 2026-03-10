cmake_minimum_required(VERSION 3.16)
project(unittests LANGUAGES CXX)

# Enable testing
enable_testing()

# Require C++17 (or higher)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Find GoogleTest (installed or via FetchContent) for 1.17.0 release commit
include(FetchContent)
FetchContent_Declare(
  googletest
  URL https://github.com/google/googletest/archive/52eb8108c5bdec04579160ae17225d66034bd723.zip
)
FetchContent_MakeAvailable(googletest)

# Prevent GoogleTest from overriding compiler/linker settings
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

# Recursively find all test source files (*.cpp) in "tests" folder
file(GLOB_RECURSE TEST_SOURCES
    CONFIGURE_DEPENDS
    ${CMAKE_CURRENT_SOURCE_DIR}/jxx_*.cpp
)

# Create a single test executable
add_executable(all_tests ${TEST_SOURCES})

add_compile_definitions(_HAS_STD_BYTE=0)

if(WIN32 AND MSVC)
    # Add the definition for all targets
        add_compile_definitions(WIN32_LEAN_AND_MEAN)
        add_definitions(-DNOMINMAX)   # Avoid min/max macro conflicts
        add_compile_options(/MP)      # Enable multi-core compilation

endif()

target_include_directories(all_tests
    PRIVATE

        ${CMAKE_SOURCE_DIR}/sdk   # Path to your headers
		${CMAKE_SOURCE_DIR}/sdk/defines
		${CMAKE_SOURCE_DIR}/sdk/lang
		${CMAKE_SOURCE_DIR}/sdk/util
		${CMAKE_SOURCE_DIR}/sdk/io
		${CMAKE_SOURCE_DIR}/sdk/net	
		${CMAKE_SOURCE_DIR}/sdk/security	
        
)

# Link GoogleTest and pthread (Linux/Unix)
target_link_libraries(all_tests
    PRIVATE
        GTest::gtest GTest::gmock GTest::gtest_main
)

if(UNIX AND NOT APPLE)
    target_link_libraries(all_tests PRIVATE dl)
endif()



# Automatically discover tests in the executable
include(GoogleTest)
gtest_discover_tests(all_tests)
