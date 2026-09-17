#include "awt/jxx.awt.PopupMenu.h"

#include <sstream>

#include "awt/jxx.awt.Component.h"
#include "lang/jxx.lang.IllegalArgumentException.h"
#include "lang/jxx.lang.NullPointerException.h"
#include "lang/jxx.lang.String.h"

namespace jxx::awt
{
    namespace
    {
        ::jxx::Ptr<::jxx::lang::String> emptyLabel()
        {
            return ::jxx::NEW<::jxx::lang::String>("");
        }
    }

    PopupMenu::PopupMenu() : PopupMenu(emptyLabel())
    {
    }

    PopupMenu::PopupMenu(const ::jxx::Ptr<::jxx::lang::String>& label)
        : Super(label != nullptr ? label : emptyLabel())
    {
    }

    void PopupMenu::addNotify()
    {
    }

    void PopupMenu::show(const ::jxx::Ptr<Component>& origin,
        ::jxx::lang::jint x,
        ::jxx::lang::jint y)
    {
        if (origin == nullptr)
            throw ::jxx::lang::NullPointerException("origin");
        if (!origin->isVisible())
            throw ::jxx::lang::IllegalArgumentException(
                "origin must be visible");

        lastOrigin_ = origin;
        lastX_ = x;
        lastY_ = y;
    }

    ::jxx::Ptr<::jxx::lang::String> PopupMenu::paramString() const
    {
        std::ostringstream out;
        out << "x=" << lastX_ << ",y=" << lastY_;
        return ::jxx::NEW<::jxx::lang::String>(out.str());
    }
}
