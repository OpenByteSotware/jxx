#include "swing/jxx.swing.JButton.h"

namespace jxx::swing
{
    JButton::JButton()
        : Super()
    {
    }

    JButton::JButton(const ::jxx::Ptr<::jxx::lang::String>& value)
        : Super(value)
    {
    }

    ::jxx::lang::jbool JButton::isDefaultButton() const
    {
        return false;
    }

    ::jxx::lang::jbool JButton::isDefaultCapable() const
    {
        return defaultCapable_;
    }

    void JButton::setDefaultCapable(::jxx::lang::jbool value)
    {
        defaultCapable_ = value;
    }
}
