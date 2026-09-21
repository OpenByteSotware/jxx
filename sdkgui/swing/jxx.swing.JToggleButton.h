#pragma once

#include "swing/jxx.swing.AbstractButton.h"

namespace jxx::swing
{
    class ButtonGroup;

    class JToggleButton : public ::jxx::lang::ClassBase<JToggleButton, AbstractButton>
    {
    public:
        using JxxSuper = AbstractButton;
        using Super = ::jxx::lang::ClassBase<JToggleButton, JxxSuper>;

        JToggleButton();
        explicit JToggleButton(const ::jxx::Ptr<::jxx::lang::String>& text);
        JToggleButton(const ::jxx::Ptr<::jxx::lang::String>& text,
            ::jxx::lang::jbool selected);

        ::jxx::lang::jbool isSelected() const;
        void setSelected(::jxx::lang::jbool selected) override;
        void doClick() override;

    private:
        friend class ButtonGroup;
        void setSelectedFromGroup(::jxx::lang::jbool selected);
        std::weak_ptr<ButtonGroup> group_;
    };
}
