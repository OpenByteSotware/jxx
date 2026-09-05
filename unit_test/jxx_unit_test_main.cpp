#include <cstdio>
#include <gtest/gtest.h>
#include "jxx.initialize.h"

int main(
    int argc,
    char** argv)
{
    std::fprintf(
        stderr,
        "[all_tests] entered main\n");
    std::fflush(stderr);

    jxx::initialize();

    std::fprintf(
        stderr,
        "[all_tests] JXX initialized\n");
    std::fflush(stderr);

    ::testing::InitGoogleTest(
        &argc,
        argv);

    std::fprintf(
        stderr,
        "[all_tests] test runtime initialized\n");
    std::fflush(stderr);

    return RUN_ALL_TESTS();
}