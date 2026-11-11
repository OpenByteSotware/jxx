cmake_minimum_required(VERSION 3.16)
project(TranspiledProject LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Build JXX through its own CMake project so all third-party
# target dependencies are preserved transitively. Application
# projects must not configure the JXX unit-test subtree.
set(BUILD_TESTING OFF CACHE BOOL "Disable dependency tests" FORCE)
set(JXX_BUILD_TESTS OFF CACHE BOOL "Disable JXX tests" FORCE)
add_subdirectory("C:/projects/SourceCode/git_repos/open_source/jxx"
                 "${CMAKE_BINARY_DIR}/jxx"
                 EXCLUDE_FROM_ALL)
if(NOT TARGET jxxsdk)
    message(FATAL_ERROR "JXX root did not define target jxxsdk")
endif()
add_library(jxx_sdk ALIAS jxxsdk)


include(CTest)
enable_testing()
find_package(GTest CONFIG REQUIRED)
add_executable(converted_tests
    unit_test/jdk8/lang/jxx_booleantest_test.cpp
    unit_test/jdk8/lang/jxx_bytetest_test.cpp
    unit_test/jdk8/lang/jxx_integertest_test.cpp
    unit_test/jdk8/lang/jxx_longtest_test.cpp
    unit_test/jdk8/lang/jxx_shorttest_test.cpp
    unit_test/jdk8/util/jxx_arraydequetest_test.cpp
    unit_test/jdk8/util/jxx_arraylisttest_test.cpp
    unit_test/jdk8/util/jxx_arraystest_test.cpp
    unit_test/jdk8/util/jxx_collectionstest_test.cpp
    unit_test/jdk8/util/jxx_comparatortest_test.cpp
    unit_test/jdk8/util/jxx_hashmaptest_test.cpp
    unit_test/jdk8/util/jxx_hashsettest_test.cpp
    unit_test/jdk8/util/jxx_linkedhashmaptest_test.cpp
    unit_test/jdk8/util/jxx_linkedhashsettest_test.cpp
    unit_test/jdk8/util/jxx_linkedlisttest_test.cpp
    unit_test/jdk8/util/jxx_objectstest_test.cpp
    unit_test/jdk8/util/jxx_optionaltest_test.cpp
    unit_test/jdk8/util/jxx_priorityqueuetest_test.cpp
    unit_test/jdk8/util/jxx_propertiestest_test.cpp
    unit_test/jdk8/util/jxx_randomtest_test.cpp
    unit_test/jdk8/util/jxx_scannertest_test.cpp
    unit_test/jdk8/util/jxx_stringtokenizertest_test.cpp
    unit_test/jdk8/util/jxx_treemaptest_test.cpp
    unit_test/jdk8/util/jxx_treesettest_test.cpp
    unit_test/jdk8/util/jxx_uuidtest_test.cpp
    unit_test/lang/jxx_synchronizedlockexampletest_test.cpp
)
target_include_directories(converted_tests PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}
    "c:/projects/SourceCode/git_repos/open_source/jxx/sdk"
)
target_link_libraries(converted_tests PRIVATE
    transpiled
    GTest::gtest_main
)
include(GoogleTest)
gtest_discover_tests(converted_tests
    DISCOVERY_MODE POST_BUILD
    DISCOVERY_TIMEOUT 60
)
