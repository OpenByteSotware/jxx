#pragma once
#include "swing/jxx.swing.JComponent.h"
#include "swing/jxx.swing.SwingConstants.h"
namespace jxx::swing
{
    class JSeparator : public ::jxx::lang::ClassBase<JSeparator, JComponent, SwingConstants>
    {
    public:
        using JxxSuper = JComponent;
        using Super = ::jxx::lang::ClassBase<JSeparator, JxxSuper, SwingConstants>;
        JSeparator();
        explicit JSeparator(::jxx::lang::jint orientation);
        ::jxx::lang::jint getOrientation() const;
        void setOrientation(::jxx::lang::jint orientation);
    private:
        ::jxx::lang::jint orientation_ = HORIZONTAL;
    };
}
