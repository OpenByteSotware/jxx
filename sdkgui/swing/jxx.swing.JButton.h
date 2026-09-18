#pragma once
#include "swing/jxx.swing.AbstractButton.h"
namespace jxx::swing
{
    class JButton : public ::jxx::lang::ClassBase<JButton, AbstractButton>
    {
    public:
        using JxxSuper = AbstractButton;
        using Super = ::jxx::lang::ClassBase<JButton, JxxSuper>;
        JButton();
        explicit JButton(const ::jxx::Ptr<::jxx::lang::String>& text);
        ~JButton() override = default;
        ::jxx::lang::jbool isDefaultButton() const;
        ::jxx::lang::jbool isDefaultCapable() const;
        void setDefaultCapable(::jxx::lang::jbool value);
    private:
        ::jxx::lang::jbool defaultCapable_ = true;
    };
}
