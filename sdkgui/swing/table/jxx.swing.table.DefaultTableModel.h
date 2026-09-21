#pragma once
#include <vector>
#include "lang/jxx.lang.IndexOutOfBoundsException.h"
#include "swing/table/jxx.swing.table.AbstractTableModel.h"
namespace jxx::swing::table
{
    class DefaultTableModel : public ::jxx::lang::ClassBase<DefaultTableModel, AbstractTableModel>
    {
    public:
        using JxxSuper = AbstractTableModel;
        using Super = ::jxx::lang::ClassBase<DefaultTableModel, JxxSuper>;
        DefaultTableModel();
        DefaultTableModel(::jxx::lang::jint rows, ::jxx::lang::jint columns);
        ::jxx::lang::jint getRowCount() const override;
        ::jxx::lang::jint getColumnCount() const override;
        ::jxx::Ptr<::jxx::lang::String> getColumnName(::jxx::lang::jint column) const override;
        ::jxx::Ptr<::jxx::lang::Object> getValueAt(::jxx::lang::jint row, ::jxx::lang::jint column) const override;
        void setValueAt(const ::jxx::Ptr<::jxx::lang::Object>& value, ::jxx::lang::jint row, ::jxx::lang::jint column) override;
        ::jxx::lang::jbool isCellEditable(::jxx::lang::jint row, ::jxx::lang::jint column) const override;
        void setRowCount(::jxx::lang::jint rows);
        void setColumnCount(::jxx::lang::jint columns);
        void addRow(const std::vector<::jxx::Ptr<::jxx::lang::Object>>& row);
        void removeRow(::jxx::lang::jint row);
        void setColumnIdentifiers(const std::vector<::jxx::Ptr<::jxx::lang::String>>& identifiers);
    private:
        void check(::jxx::lang::jint row, ::jxx::lang::jint column) const;
        std::vector<std::vector<::jxx::Ptr<::jxx::lang::Object>>> data_;
        std::vector<::jxx::Ptr<::jxx::lang::String>> columns_;
    };
}
