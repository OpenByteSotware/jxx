#include "awt/jxx.awt.Frame.h"
#include "gui/internal/jxx.gui.internal.NativeWindow.h"

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
