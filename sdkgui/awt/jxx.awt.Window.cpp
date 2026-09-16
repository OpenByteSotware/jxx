#include "awt/jxx.awt.Window.h"

#include "gui/internal/jxx.gui.internal.NativeWindow.h"

namespace jxx::awt
{
    Window::Window()
        : active_(false)
        , displayable_(false)
    {
        Component::setVisible(false);
    }

    Window::~Window()
    {
        dispose();
    }

    void Window::ensureNativeWindow()
    {
    }

    void Window::setVisible(::jxx::lang::jbool visible)
    {
        if (visible)
        {
            ensureNativeWindow();
        }

        Component::setVisible(visible);

        if (nativeWindow_ != nullptr)
        {
            if (visible)
            {
                nativeWindow_->setBounds(
                    getX(), getY(), getWidth(), getHeight());
                nativeWindow_->show();
                displayable_ = true;
            }
            else
            {
                nativeWindow_->hide();
            }
        }
    }

    void Window::setBounds(const ::jxx::Ptr<Rectangle>& rectangle)
    {
        if (rectangle != nullptr)
        {
            setBounds(
                rectangle->x,
                rectangle->y,
                rectangle->width,
                rectangle->height);
        }
    }

    void Window::setBounds(
        ::jxx::lang::jint x,
        ::jxx::lang::jint y,
        ::jxx::lang::jint width,
        ::jxx::lang::jint height)
    {
        Component::setBounds(x, y, width, height);

        if (nativeWindow_ != nullptr)
        {
            nativeWindow_->setBounds(x, y, width, height);
        }
    }

    void Window::pack()
    {
        validate();
        ensureNativeWindow();
        displayable_ = nativeWindow_ != nullptr;
    }

    void Window::dispose()
    {
        if (nativeWindow_ != nullptr)
        {
            nativeWindow_->destroy();
            nativeWindow_.reset();
        }

        active_ = false;
        displayable_ = false;
        Component::setVisible(false);
    }

    void Window::toFront()
    {
        ensureNativeWindow();
        if (nativeWindow_ != nullptr)
        {
            nativeWindow_->toFront();
        }
    }

    void Window::toBack()
    {
        if (nativeWindow_ != nullptr)
        {
            nativeWindow_->toBack();
        }
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
