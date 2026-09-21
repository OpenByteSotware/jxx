#pragma once

#include <vector>

#include "io/jxx.io.SerializableI.h"
#include "swing/table/jxx.swing.table.TableModel.h"

namespace jxx::swing::event
{
    class TableModelEvent;
    class TableModelListener;
}

namespace jxx::swing::table
{
    class AbstractTableModel : public ::jxx::lang::ClassBase<
        AbstractTableModel, ::jxx::lang::Object,
        TableModel, ::jxx::io::SerializableI>
    {
    public:
        using JxxSuper = ::jxx::lang::Object;
        using Super = ::jxx::lang::ClassBase<
            AbstractTableModel, JxxSuper,
            TableModel, ::jxx::io::SerializableI>;

        AbstractTableModel();
        ~AbstractTableModel() override = default;

        ::jxx::Ptr<::jxx::lang::String> getColumnName(
            ::jxx::lang::jint column) const override;
        ::jxx::lang::jbool isCellEditable(
            ::jxx::lang::jint row,
            ::jxx::lang::jint column) const override;
        void setValueAt(
            const ::jxx::Ptr<::jxx::lang::Object>& value,
            ::jxx::lang::jint row,
            ::jxx::lang::jint column) override;
        ::jxx::lang::jint findColumn(
            const ::jxx::Ptr<::jxx::lang::String>& columnName) const;
        void addTableModelListener(
            const ::jxx::Ptr<::jxx::swing::event::TableModelListener>&
                listener) override;
        void removeTableModelListener(
            const ::jxx::Ptr<::jxx::swing::event::TableModelListener>&
                listener) override;

        void fireTableDataChanged();
        void fireTableStructureChanged();
        void fireTableRowsInserted(
            ::jxx::lang::jint firstRow,
            ::jxx::lang::jint lastRow);
        void fireTableRowsUpdated(
            ::jxx::lang::jint firstRow,
            ::jxx::lang::jint lastRow);
        void fireTableRowsDeleted(
            ::jxx::lang::jint firstRow,
            ::jxx::lang::jint lastRow);
        void fireTableCellUpdated(
            ::jxx::lang::jint row,
            ::jxx::lang::jint column);
        void fireTableChanged(
            const ::jxx::Ptr<::jxx::swing::event::TableModelEvent>& event);

        void writeObject(
            const ::jxx::Ptr<::jxx::io::ObjectOutputStream>& out) override;
        void readObject(
            const ::jxx::Ptr<::jxx::io::ObjectInputStream>& in) override;
        void readObjectNoData() override;

    protected:
        std::vector<::jxx::Ptr<
            ::jxx::swing::event::TableModelListener>> listeners_;
    };
}
