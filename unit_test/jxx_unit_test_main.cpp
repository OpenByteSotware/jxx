#include <cstdio>

#include <gtest/gtest.h>

int main(
    int argc,
    char** argv)
{
    std::fprintf(
        stderr,
        "[all_tests] entered main\n");

    std::fflush(stderr);

    ::testing::InitGoogleTest(
        &argc,
        argv);

    std::fprintf(
        stderr,
        "[all_tests] initialized test runtime\n");

    std::fflush(stderr);

    return RUN_ALL_TESTS();
}
