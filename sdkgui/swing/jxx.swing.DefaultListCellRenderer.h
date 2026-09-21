#pragma once

#include "swing/jxx.swing.JLabel.h"
#include "swing/jxx.swing.ListCellRenderer.h"

namespace jxx::swing
{
    class DefaultListCellRenderer : public ::jxx::lang::ClassBase<
        DefaultListCellRenderer, JLabel,
        ListCellRenderer<::jxx::lang::Object>>
    {
    public:
        using JxxSuper = JLabel;
        using Super = ::jxx::lang::ClassBase<DefaultListCellRenderer,
            JxxSuper, ListCellRenderer<::jxx::lang::Object>>;

        DefaultListCellRenderer();
        ~DefaultListCellRenderer() override = default;

        ::jxx::Ptr<::jxx::awt::Component> getListCellRendererComponent(
            const ::jxx::Ptr<JList<::jxx::lang::Object>>& list,
            const ::jxx::Ptr<::jxx::lang::Object>& value,
            ::jxx::lang::jint index,
            ::jxx::lang::jbool isSelected,
            ::jxx::lang::jbool cellHasFocus) override;
    };
}
