#pragma once
#include "swing/jxx.swing.JMenuItem.h"
namespace jxx::swing
{
    class JCheckBoxMenuItem : public ::jxx::lang::ClassBase<JCheckBoxMenuItem, JMenuItem>
    {
    public:
        using JxxSuper = JMenuItem;
        using Super = ::jxx::lang::ClassBase<JCheckBoxMenuItem, JxxSuper>;
        JCheckBoxMenuItem();
        explicit JCheckBoxMenuItem(const ::jxx::Ptr<::jxx::lang::String>& text);
        JCheckBoxMenuItem(const ::jxx::Ptr<::jxx::lang::String>& text, ::jxx::lang::jbool selected);
        ::jxx::lang::jbool getState() const;
        void setState(::jxx::lang::jbool selected);
        ::jxx::lang::jbool isSelected() const;
        void setSelected(::jxx::lang::jbool selected);
    private:
        ::jxx::lang::jbool selected_ = false;
    };
}
