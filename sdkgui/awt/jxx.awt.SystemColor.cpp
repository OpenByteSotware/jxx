#include "awt/jxx.awt.SystemColor.h"

#include <sstream>
#include <wx/colour.h>
#include <wx/settings.h>

#include "gui/internal/jxx.gui.internal.GuiRuntime.h"
#include "io/jxx.io.ObjectInputStream.h"
#include "io/jxx.io.ObjectOutputStream.h"
#include "lang/jxx.lang.IllegalArgumentException.h"

namespace jxx::awt
{
    namespace
    {
        wxSystemColour nativeIndex(::jxx::lang::jint index)
        {
            static constexpr wxSystemColour values[] = {
                wxSYS_COLOUR_DESKTOP, wxSYS_COLOUR_ACTIVECAPTION,
                wxSYS_COLOUR_CAPTIONTEXT, wxSYS_COLOUR_ACTIVEBORDER,
                wxSYS_COLOUR_INACTIVECAPTION,
                wxSYS_COLOUR_INACTIVECAPTIONTEXT,
                wxSYS_COLOUR_INACTIVEBORDER, wxSYS_COLOUR_WINDOW,
                wxSYS_COLOUR_WINDOWFRAME, wxSYS_COLOUR_WINDOWTEXT,
                wxSYS_COLOUR_MENU, wxSYS_COLOUR_MENUTEXT,
                wxSYS_COLOUR_WINDOW, wxSYS_COLOUR_WINDOWTEXT,
                wxSYS_COLOUR_HIGHLIGHT, wxSYS_COLOUR_HIGHLIGHTTEXT,
                wxSYS_COLOUR_GRAYTEXT, wxSYS_COLOUR_BTNFACE,
                wxSYS_COLOUR_BTNTEXT, wxSYS_COLOUR_BTNHIGHLIGHT,
                wxSYS_COLOUR_3DLIGHT, wxSYS_COLOUR_BTNSHADOW,
                wxSYS_COLOUR_3DDKSHADOW, wxSYS_COLOUR_SCROLLBAR,
                wxSYS_COLOUR_INFOBK, wxSYS_COLOUR_INFOTEXT
            };
            return values[index];
        }
    }

    ::jxx::lang::jint SystemColor::rgbFor(::jxx::lang::jint index)
    {
        if (!::jxx::gui::internal::GuiRuntime::ensureInitialized())
            return static_cast<::jxx::lang::jint>(0xff000000u);
        const auto color = wxSystemSettings::GetColour(nativeIndex(index));
        return static_cast<::jxx::lang::jint>(0xff000000u |
            (static_cast<unsigned int>(color.Red()) << 16) |
            (static_cast<unsigned int>(color.Green()) << 8) |
            static_cast<unsigned int>(color.Blue()));
    }

    SystemColor::SystemColor(::jxx::lang::jint index)
        : Super(rgbFor(index)), index_(index)
    {
    }

    ::jxx::Ptr<SystemColor> SystemColor::create(::jxx::lang::jint index)
    {
        return ::jxx::NEW<SystemColor>(index);
    }

    ::jxx::lang::jint SystemColor::getRGB() const
    {
        return rgbFor(index_);
    }

    ::jxx::Ptr<::jxx::lang::String> SystemColor::toString() const
    {
        std::ostringstream out;
        out << "jxx.awt.SystemColor[i=" << index_ << "]";
        return ::jxx::NEW<::jxx::lang::String>(out.str());
    }

    void SystemColor::writeObject(
        const ::jxx::Ptr<::jxx::io::ObjectOutputStream>& out)
    {
        out->writeInt(index_);
    }

    void SystemColor::readObject(
        const ::jxx::Ptr<::jxx::io::ObjectInputStream>& in)
    {
        const auto value = in->readInt();
        if (value < 0 || value >= NUM_COLORS)
            throw ::jxx::lang::IllegalArgumentException("index");
        index_ = value;
    }

    void SystemColor::readObjectNoData()
    {
        index_ = WINDOW;
    }

#define JXX_SYSTEM_COLOR(NAME, INDEX) \
    ::jxx::Ptr<SystemColor> SystemColor::NAME = SystemColor::create(INDEX)
    JXX_SYSTEM_COLOR(desktop, DESKTOP);
    JXX_SYSTEM_COLOR(activeCaption, ACTIVE_CAPTION);
    JXX_SYSTEM_COLOR(activeCaptionText, ACTIVE_CAPTION_TEXT);
    JXX_SYSTEM_COLOR(activeCaptionBorder, ACTIVE_CAPTION_BORDER);
    JXX_SYSTEM_COLOR(inactiveCaption, INACTIVE_CAPTION);
    JXX_SYSTEM_COLOR(inactiveCaptionText, INACTIVE_CAPTION_TEXT);
    JXX_SYSTEM_COLOR(inactiveCaptionBorder, INACTIVE_CAPTION_BORDER);
    JXX_SYSTEM_COLOR(window, WINDOW);
    JXX_SYSTEM_COLOR(windowBorder, WINDOW_BORDER);
    JXX_SYSTEM_COLOR(windowText, WINDOW_TEXT);
    JXX_SYSTEM_COLOR(menu, MENU);
    JXX_SYSTEM_COLOR(menuText, MENU_TEXT);
    JXX_SYSTEM_COLOR(text, TEXT);
    JXX_SYSTEM_COLOR(textText, TEXT_TEXT);
    JXX_SYSTEM_COLOR(textHighlight, TEXT_HIGHLIGHT);
    JXX_SYSTEM_COLOR(textHighlightText, TEXT_HIGHLIGHT_TEXT);
    JXX_SYSTEM_COLOR(textInactiveText, TEXT_INACTIVE_TEXT);
    JXX_SYSTEM_COLOR(control, CONTROL);
    JXX_SYSTEM_COLOR(controlText, CONTROL_TEXT);
    JXX_SYSTEM_COLOR(controlHighlight, CONTROL_HIGHLIGHT);
    JXX_SYSTEM_COLOR(controlLtHighlight, CONTROL_LT_HIGHLIGHT);
    JXX_SYSTEM_COLOR(controlShadow, CONTROL_SHADOW);
    JXX_SYSTEM_COLOR(controlDkShadow, CONTROL_DK_SHADOW);
    JXX_SYSTEM_COLOR(scrollbar, SCROLLBAR);
    JXX_SYSTEM_COLOR(info, INFO);
    JXX_SYSTEM_COLOR(infoText, INFO_TEXT);
#undef JXX_SYSTEM_COLOR
}
