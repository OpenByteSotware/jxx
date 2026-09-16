#include "awt/jxx.awt.Window.h"
#include "gui/internal/jxx.gui.internal.NativeWindow.h"

namespace jxx::awt
{
    Window::Window()
        : active_(false)
        , displayable_(false)
    {
    }

    Window::~Window() = default;

    void Window::pack()
    {
        validate();
    }

    void Window::dispose()
    {
        if (nativeWindow_ != nullptr)
        {
            nativeWindow_->hide();
        }

        active_ = false;
        displayable_ = false;
        nativeWindow_.reset();
    }

    void Window::toFront()
    {
    }

    void Window::toBack()
    {
    }

    ::jxx::lang::jbool Window::isActive() const
    {
        return active_;
    }

    ::jxx::lang::jbool Window::isDisplayable() const
    {
        return displayable_;
    }
}
