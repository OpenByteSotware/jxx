#include "gui/internal/jxx.gui.internal.WxFramePeer.h"

#include <wx/frame.h>
#include <wx/string.h>
#include "gui/internal/jxx.gui.internal.GuiRuntime.h"

namespace jxx::gui::internal
{
    namespace
    {
        wxString nativeText(const ::jxx::Ptr<::jxx::lang::String>& value)
        {
            return value != nullptr
                ? wxString::FromUTF8(value->utf8().c_str())
                : wxString();
        }
    }

    WxFramePeer::WxFramePeer(
        const ::jxx::Ptr<::jxx::lang::String>& title)
        : frame_(nullptr)
        , title_(title != nullptr
            ? title
            : ::jxx::NEW<::jxx::lang::String>(""))
    {
        if (GuiRuntime::ensureInitialized())
        {
            frame_ = new wxFrame(
                nullptr,
                wxID_ANY,
                nativeText(title_),
                wxDefaultPosition,
                wxDefaultSize,
                wxDEFAULT_FRAME_STYLE);
        }
    }

    WxFramePeer::~WxFramePeer()
    {
        destroy();
    }

    void WxFramePeer::show()
    {
        if (frame_ != nullptr)
        {
            frame_->Show(true);
        }
    }

    void WxFramePeer::hide()
    {
        if (frame_ != nullptr)
        {
            frame_->Show(false);
        }
    }

    void WxFramePeer::destroy()
    {
        if (frame_ != nullptr)
        {
            frame_->Destroy();
            frame_ = nullptr;
        }
    }

    void WxFramePeer::toFront()
    {
        if (frame_ != nullptr)
        {
            frame_->Raise();
        }
    }

    void WxFramePeer::toBack()
    {
        if (frame_ != nullptr)
        {
            frame_->Lower();
        }
    }

    void WxFramePeer::setBounds(
        ::jxx::lang::jint x,
        ::jxx::lang::jint y,
        ::jxx::lang::jint width,
        ::jxx::lang::jint height)
    {
        if (frame_ != nullptr)
        {
            frame_->SetSize(x, y, width, height);
        }
    }

    void WxFramePeer::setTitle(
        const ::jxx::Ptr<::jxx::lang::String>& title)
    {
        title_ = title != nullptr
            ? title
            : ::jxx::NEW<::jxx::lang::String>("");

        if (frame_ != nullptr)
        {
            frame_->SetTitle(nativeText(title_));
        }
    }

    ::jxx::Ptr<::jxx::lang::String> WxFramePeer::getTitle() const
    {
        return title_;
    }

    ::jxx::lang::jbool WxFramePeer::isShown() const
    {
        return frame_ != nullptr && frame_->IsShown();
    }
}
