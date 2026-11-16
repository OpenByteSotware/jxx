add_executable(jxx_lang_object_bench
    jxx_lang_object_bench.cpp
)

target_compile_features(jxx_lang_object_bench PRIVATE cxx_std_17)
target_link_libraries(jxx_lang_object_bench
    PRIVATE
        jxxsdk
        benchmark::benchmark
        benchmark::benchmark_main
)

if(MSVC)
    target_compile_options(jxx_lang_object_bench PRIVATE /utf-8 /permissive- /W4 /bigobj)
elseif(CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang|AppleClang")
    target_compile_options(jxx_lang_object_bench PRIVATE -Wall -Wextra -Wpedantic)
endif()

set_target_properties(jxx_lang_object_bench PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bench"
)
