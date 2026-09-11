add_executable(jxx_lang_l1_openjdk8_tests
    jxx.lang.test.Boolean.GetBoolean.cpp
)

target_compile_features(jxx_lang_l1_openjdk8_tests PRIVATE cxx_std_17)
target_link_libraries(jxx_lang_l1_openjdk8_tests PRIVATE jxxsdk GTest::gtest_main)

target_include_directories(jxx_lang_l1_openjdk8_tests PRIVATE
    ${PROJECT_SOURCE_DIR}
)

include(GoogleTest)
gtest_discover_tests(jxx_lang_l1_openjdk8_tests)
