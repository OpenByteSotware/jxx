cmake_minimum_required(VERSION 3.14)
project(jxx_object_demo LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_library(jxx_object INTERFACE)
target_include_directories(jxx_object INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}/../sdk/lang/)

add_executable(demo
    main.cpp
    Person.h
    ../sdk/lang/jxx_Integer.h
)
target_include_directories(demo PRIVATE include)
target_link_libraries(demo PRIVATE jxx_object)