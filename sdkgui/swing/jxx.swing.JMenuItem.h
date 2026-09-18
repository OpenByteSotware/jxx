#pragma once
#include "swing/jxx.swing.AbstractButton.h"
namespace jxx::swing
{
    class JMenuItem : public ::jxx::lang::ClassBase<JMenuItem, AbstractButton>
    {
    public:
        using JxxSuper = AbstractButton;
        using Super = ::jxx::lang::ClassBase<JMenuItem, JxxSuper>;
        JMenuItem();
        explicit JMenuItem(const ::jxx::Ptr<::jxx::lang::String>& text);
        ~JMenuItem() override = default;
        void setArmed(::jxx::lang::jbool armed);
        ::jxx::lang::jbool isArmed() const;
        void menuSelectionChanged(::jxx::lang::jbool included);
    private:
        ::jxx::lang::jbool armed_ = false;
    };
}
