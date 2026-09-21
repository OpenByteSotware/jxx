#pragma once

#include "lang/jxx.lang.ClassInfo.h"

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
    class TableCellRenderer : public ::jxx::lang::InterfaceBase<
        TableCellRenderer>
    {
    public:
        ~TableCellRenderer() override = default;

        virtual ::jxx::Ptr<::jxx::awt::Component>
            getTableCellRendererComponent(
                const ::jxx::Ptr<::jxx::swing::JTable>& table,
                const ::jxx::Ptr<::jxx::lang::Object>& value,
                ::jxx::lang::jbool isSelected,
                ::jxx::lang::jbool hasFocus,
                ::jxx::lang::jint row,
                ::jxx::lang::jint column) = 0;
    };
}
