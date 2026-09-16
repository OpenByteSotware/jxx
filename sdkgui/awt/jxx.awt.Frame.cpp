#include "awt/jxx.awt.Frame.h"

#include "gui/internal/jxx.gui.internal.NativeWindow.h"
#include "gui/internal/jxx.gui.internal.WxFramePeer.h"

namespace jxx::awt
{
    Frame::Frame()
        : title_(::jxx::NEW<::jxx::lang::String>(""))
    {
    }

    Frame::Frame(const ::jxx::Ptr<::jxx::lang::String>& title)
        : title_(title != nullptr
            ? title
            : ::jxx::NEW<::jxx::lang::String>(""))
    {
    }

    void Frame::ensureNativeWindow()
    {
        if (nativeWindow_ == nullptr)
        {
            nativeWindow_ =
                ::jxx::NEW<::jxx::gui::internal::WxFramePeer>(title_);

            nativeWindow_->setBounds(
                getX(), getY(), getWidth(), getHeight());

            displayable_ = true;
        }
    }

    void Frame::setTitle(
        const ::jxx::Ptr<::jxx::lang::String>& title)
    {
        title_ = title != nullptr
            ? title
            : ::jxx::NEW<::jxx::lang::String>("");

        if (nativeWindow_ != nullptr)
        {
            nativeWindow_->setTitle(title_);
        }
    }

    ::jxx::Ptr<::jxx::lang::String> Frame::getTitle() const
    {
        return title_;
    }
}
