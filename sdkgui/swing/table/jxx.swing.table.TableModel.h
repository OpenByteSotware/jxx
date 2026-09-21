#pragma once
#include "lang/jxx.lang.ClassInfo.h"
#include "lang/jxx.lang.Object.h"
#include "lang/jxx.lang.String.h"
namespace jxx::swing::table
{
    class TableModel : public ::jxx::lang::InterfaceBase<TableModel>
    {
    public:
        ~TableModel() override = default;
        virtual ::jxx::lang::jint getRowCount() const = 0;
        virtual ::jxx::lang::jint getColumnCount() const = 0;
        virtual ::jxx::Ptr<::jxx::lang::String> getColumnName(::jxx::lang::jint column) const = 0;
        virtual ::jxx::Ptr<::jxx::lang::Object> getValueAt(::jxx::lang::jint row, ::jxx::lang::jint column) const = 0;
        virtual void setValueAt(const ::jxx::Ptr<::jxx::lang::Object>& value, ::jxx::lang::jint row, ::jxx::lang::jint column) = 0;
        virtual ::jxx::lang::jbool isCellEditable(::jxx::lang::jint row, ::jxx::lang::jint column) const = 0;
    };
}
