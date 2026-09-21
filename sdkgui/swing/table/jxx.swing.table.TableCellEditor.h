#pragma once

#include "swing/jxx.swing.CellEditor.h"

namespace jxx::awt
{
    class Component;
}

namespace jxx::lang
{
    class Object;
}

namespace jxx::swing
{
    class JTable;
}

namespace jxx::swing::table
{
    class TableCellEditor : public ::jxx::lang::InterfaceBase<
        TableCellEditor, ::jxx::swing::CellEditor>
    {
    public:
        ~TableCellEditor() override = default;

        virtual ::jxx::Ptr<::jxx::awt::Component>
            getTableCellEditorComponent(
                const ::jxx::Ptr<::jxx::swing::JTable>& table,
                const ::jxx::Ptr<::jxx::lang::Object>& value,
                ::jxx::lang::jbool isSelected,
                ::jxx::lang::jint row,
                ::jxx::lang::jint column) = 0;
    };
}
