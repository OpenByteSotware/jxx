#include "jxx.gui.initialize.h"

#include "gui/internal/jxx.gui.internal.GuiRuntime.h"

#include <mutex>
#include <stdexcept>

namespace jxx::gui
{
    void initialize()
    {
        static std::once_flag flag;
        std::call_once(flag, []()
        {
            if (!::jxx::gui::internal::GuiRuntime::ensureInitialized())
            {
                throw std::runtime_error("Unable to initialize the JXX GUI runtime");
            }
        });
    }
}
