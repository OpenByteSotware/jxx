#include "awt/jxx.awt.MouseInfo.h"

#include <wx/settings.h>
#include <wx/utils.h>

#include "awt/jxx.awt.Point.h"
#include "awt/jxx.awt.PointerInfo.h"
#include "gui/internal/jxx.gui.internal.GuiRuntime.h"

namespace jxx::awt
{
    ::jxx::Ptr<PointerInfo> MouseInfo::getPointerInfo()
    {
        if (!::jxx::gui::internal::GuiRuntime::ensureInitialized())
            return nullptr;
        int x = 0;
        int y = 0;
        wxGetMousePosition(&x, &y);
        return ::jxx::NEW<PointerInfo>(nullptr,
            ::jxx::NEW<Point>(x, y));
    }

    ::jxx::lang::jint MouseInfo::getNumberOfButtons()
    {
        if (!::jxx::gui::internal::GuiRuntime::ensureInitialized())
            return -1;
        return static_cast<::jxx::lang::jint>(
            wxSystemSettings::GetMetric(wxSYS_MOUSE_BUTTONS));
    }
}
