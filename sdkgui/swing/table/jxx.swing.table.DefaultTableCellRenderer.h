#pragma once

#include "io/jxx.io.SerializableI.h"
#include "swing/jxx.swing.JLabel.h"
#include "swing/table/jxx.swing.table.TableCellRenderer.h"

namespace jxx::swing::table
{
    class DefaultTableCellRenderer : public ::jxx::lang::ClassBase<
        DefaultTableCellRenderer, ::jxx::swing::JLabel,
        TableCellRenderer, ::jxx::io::SerializableI>
    {
    public:
        using JxxSuper = ::jxx::swing::JLabel;
        using Super = ::jxx::lang::ClassBase<
            DefaultTableCellRenderer, JxxSuper,
            TableCellRenderer, ::jxx::io::SerializableI>;

        DefaultTableCellRenderer();
        ~DefaultTableCellRenderer() override = default;

        void setValue(const ::jxx::Ptr<::jxx::lang::Object>& value);
        ::jxx::Ptr<::jxx::awt::Component>
            getTableCellRendererComponent(
                const ::jxx::Ptr<::jxx::swing::JTable>& table,
                const ::jxx::Ptr<::jxx::lang::Object>& value,
                ::jxx::lang::jbool isSelected,
                ::jxx::lang::jbool hasFocus,
                ::jxx::lang::jint row,
                ::jxx::lang::jint column) override;

        void writeObject(
            const ::jxx::Ptr<::jxx::io::ObjectOutputStream>& out) override;
        void readObject(
            const ::jxx::Ptr<::jxx::io::ObjectInputStream>& in) override;
        void readObjectNoData() override;
    };
}
