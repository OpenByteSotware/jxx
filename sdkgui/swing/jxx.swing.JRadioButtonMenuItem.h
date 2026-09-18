#pragma once
#include "swing/jxx.swing.JMenuItem.h"
namespace jxx::swing
{
    class JRadioButtonMenuItem : public ::jxx::lang::ClassBase<JRadioButtonMenuItem, JMenuItem>
    {
    public:
        using JxxSuper = JMenuItem;
        using Super = ::jxx::lang::ClassBase<JRadioButtonMenuItem, JxxSuper>;
        JRadioButtonMenuItem();
        explicit JRadioButtonMenuItem(const ::jxx::Ptr<::jxx::lang::String>& text);
        JRadioButtonMenuItem(const ::jxx::Ptr<::jxx::lang::String>& text, ::jxx::lang::jbool selected);
        ::jxx::lang::jbool isSelected() const;
        void setSelected(::jxx::lang::jbool selected);
    private:
        ::jxx::lang::jbool selected_ = false;
    };
}
