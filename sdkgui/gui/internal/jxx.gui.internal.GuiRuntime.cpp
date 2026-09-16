#include "gui/internal/jxx.gui.internal.GuiRuntime.h"

#include <wx/app.h>
#include <wx/init.h>
#include <wx/thread.h>

namespace jxx::gui::internal
{
    ::jxx::lang::jbool GuiRuntime::ensureInitialized()
    {
        static wxInitializer initializer;
        return initializer.IsOk();
    }

    ::jxx::lang::jint GuiRuntime::run()
    {
        if (!ensureInitialized())
        {
            return -1;
        }

        return wxTheApp != nullptr
            ? static_cast<::jxx::lang::jint>(wxTheApp->OnRun())
            : 0;
    }

    void GuiRuntime::exit()
    {
        if (wxTheApp != nullptr)
        {
            wxTheApp->ExitMainLoop();
        }
    }

    ::jxx::lang::jbool GuiRuntime::isEventDispatchThread()
    {
        return wxIsMainThread();
    }
}
