#pragma once
#include "swing/jxx.swing.JToggleButton.h"
namespace jxx::swing
{
    class JRadioButton : public ::jxx::lang::ClassBase<JRadioButton, JToggleButton>
    {
    public:
        using JxxSuper = JToggleButton;
        using Super = ::jxx::lang::ClassBase<JRadioButton, JxxSuper>;
        JRadioButton();
        explicit JRadioButton(const ::jxx::Ptr<::jxx::lang::String>& text);
        JRadioButton(const ::jxx::Ptr<::jxx::lang::String>& text, ::jxx::lang::jbool selected);
    };
}
