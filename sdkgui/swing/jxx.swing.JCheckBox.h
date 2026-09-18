#pragma once
#include "swing/jxx.swing.JToggleButton.h"
namespace jxx::swing
{
    class JCheckBox : public ::jxx::lang::ClassBase<JCheckBox, JToggleButton>
    {
    public:
        using JxxSuper = JToggleButton;
        using Super = ::jxx::lang::ClassBase<JCheckBox, JxxSuper>;
        JCheckBox();
        explicit JCheckBox(const ::jxx::Ptr<::jxx::lang::String>& text);
        JCheckBox(const ::jxx::Ptr<::jxx::lang::String>& text, ::jxx::lang::jbool selected);
        void setBorderPaintedFlat(::jxx::lang::jbool flat);
        ::jxx::lang::jbool isBorderPaintedFlat() const;
    private:
        ::jxx::lang::jbool borderPaintedFlat_ = false;
    };
}
