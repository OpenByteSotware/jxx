#include <cstdio>
#include <iostream>

#include <gtest/gtest.h>

#include "jxx.initialize.h"

#if defined(_WIN32) && defined(_DEBUG)
#include <crtdbg.h>
#endif

namespace
{
#if defined(_WIN32) && defined(_DEBUG)

void initializeMemoryDiagnostics()
{
    int flags =
        _CrtSetDbgFlag(_CRTDBG_REPORT_FLAG);

    flags |= _CRTDBG_ALLOC_MEM_DF;
    flags |= _CRTDBG_LEAK_CHECK_DF;

    _CrtSetDbgFlag(flags);

    _CrtSetReportMode(
        _CRT_WARN,
        _CRTDBG_MODE_DEBUG | _CRTDBG_MODE_FILE);

    _CrtSetReportFile(
        _CRT_WARN,
        _CRTDBG_FILE_STDERR);

    _CrtSetReportMode(
        _CRT_ERROR,
        _CRTDBG_MODE_DEBUG | _CRTDBG_MODE_FILE);

    _CrtSetReportFile(
        _CRT_ERROR,
        _CRTDBG_FILE_STDERR);

    _CrtSetReportMode(
        _CRT_ASSERT,
        _CRTDBG_MODE_DEBUG | _CRTDBG_MODE_FILE);

    _CrtSetReportFile(
        _CRT_ASSERT,
        _CRTDBG_FILE_STDERR);

    std::fprintf(
        stderr,
        "[all_tests] Windows CRT memory diagnostics enabled\n");

    std::fflush(stderr);
}

#else

void initializeMemoryDiagnostics()
{
#if defined(__has_feature)
#if __has_feature(address_sanitizer)
    std::fprintf(
        stderr,
        "[all_tests] AddressSanitizer instrumentation enabled\n");
#else
    std::fprintf(
        stderr,
        "[all_tests] AddressSanitizer instrumentation not detected\n");
#endif
#elif defined(__SANITIZE_ADDRESS__)
    std::fprintf(
        stderr,
        "[all_tests] AddressSanitizer instrumentation enabled\n");
#else
    std::fprintf(
        stderr,
        "[all_tests] AddressSanitizer instrumentation not detected\n");
#endif

    std::fflush(stderr);
}

#endif

class JxxGlobalTestEnvironment final
    : public ::testing::Environment
{
public:
    void SetUp() override
    {
        std::fprintf(
            stderr,
            "[all_tests] global test environment started\n");

        std::fflush(stderr);
    }

    void TearDown() override
    {
        std::fprintf(
            stderr,
            "[all_tests] global test environment completed\n");

        std::fflush(stderr);
    }
};

} // namespace

int main(
    int argc,
    char** argv)
{
    std::cerr
        << "ENTERED all_tests main"
        << std::endl;

    std::fprintf(
        stderr,
        "[all_tests] entered main\n");

    std::fflush(stderr);

    initializeMemoryDiagnostics();

    jxx::initialize();

    std::fprintf(
        stderr,
        "[all_tests] JXX initialized\n");

    std::fflush(stderr);

    ::testing::InitGoogleTest(
        &argc,
        argv);

    ::testing::AddGlobalTestEnvironment(
        new JxxGlobalTestEnvironment());

    std::fprintf(
        stderr,
        "[all_tests] test runtime initialized\n");

    std::fflush(stderr);

    const int testResult =
        RUN_ALL_TESTS();

    std::fprintf(
        stderr,
        "[all_tests] test runtime completed with result %d\n",
        testResult);

    std::fflush(stderr);

#if defined(_WIN32) && defined(_DEBUG)
    /*
     * Do not call _CrtDumpMemoryLeaks() here.
     *
     * _CRTDBG_LEAK_CHECK_DF performs the dump during process exit,
     * allowing objects with normal process lifetime additional
     * opportunity to be destroyed.
     */
    std::fprintf(
        stderr,
        "[all_tests] Windows CRT leak report will run at process exit\n");

    std::fflush(stderr);
#endif

    return testResult;
}