#include <cstdio>
#include <gtest/gtest.h>

#include <wx/app.h>
#include <wx/init.h>
#include <wx/timer.h>
#include "jxx.initialize.h"
#include "awt/jxx.awt.Button.h"
#include "awt/jxx.awt.Dialog.h"
#include "awt/jxx.awt.FlowLayout.h"
#include "awt/jxx.awt.Frame.h"
#include "lang/jxx.lang.String.h"

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
    ::testing::InitGoogleTest(&argc, argv);
    if (!wxEntryStart(argc, argv)) return EXIT_FAILURE;
    int result = EXIT_FAILURE;
    if (wxTheApp != nullptr && wxTheApp->CallOnInit()) {
        result = RUN_ALL_TESTS();
        wxTheApp->ProcessPendingEvents();
        wxTheApp->OnExit();
    }
    wxEntryCleanup();
    return result;

}