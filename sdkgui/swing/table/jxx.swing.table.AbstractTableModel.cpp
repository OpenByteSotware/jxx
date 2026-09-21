#include "swing/table/jxx.swing.table.AbstractTableModel.h"

#include <algorithm>
#include <string>

#include "swing/event/jxx.swing.event.TableModelEvent.h"
#include "swing/event/jxx.swing.event.TableModelListener.h"

namespace jxx::swing::table
{
    AbstractTableModel::AbstractTableModel() : Super()
    {
    }

    ::jxx::Ptr<::jxx::lang::String> AbstractTableModel::getColumnName(
        ::jxx::lang::jint column) const
    {
        if (column < 0) return ::jxx::NEW<::jxx::lang::String>("");
        std::string name;
        auto value = column;
        do
        {
            name.insert(name.begin(),
                static_cast<char>('A' + value % 26));
            value = value / 26 - 1;
        }
        while (value >= 0);
        return ::jxx::NEW<::jxx::lang::String>(name);
    }

    ::jxx::lang::jbool AbstractTableModel::isCellEditable(
        ::jxx::lang::jint,
        ::jxx::lang::jint) const
    {
        return false;
    }

    void AbstractTableModel::setValueAt(
        const ::jxx::Ptr<::jxx::lang::Object>&,
        ::jxx::lang::jint,
        ::jxx::lang::jint)
    {
    }

    ::jxx::lang::jint AbstractTableModel::findColumn(
        const ::jxx::Ptr<::jxx::lang::String>& columnName) const
    {
        if (columnName == nullptr) return -1;
        for (::jxx::lang::jint column = 0;
             column < getColumnCount(); ++column)
        {
            const auto candidate = getColumnName(column);
            if (candidate != nullptr
                && candidate->equals(
                    ::jxx::CAST<::jxx::lang::Object>(columnName)))
                return column;
        }
        return -1;
    }

    void AbstractTableModel::addTableModelListener(
        const ::jxx::Ptr<::jxx::swing::event::TableModelListener>& listener)
    {
        if (listener != nullptr) listeners_.push_back(listener);
    }

    void AbstractTableModel::removeTableModelListener(
        const ::jxx::Ptr<::jxx::swing::event::TableModelListener>& listener)
    {
        listeners_.erase(std::remove(listeners_.begin(), listeners_.end(),
            listener), listeners_.end());
    }

    void AbstractTableModel::fireTableDataChanged()
    {
        fireTableChanged(::jxx::NEW<
            ::jxx::swing::event::TableModelEvent>(
                ::jxx::CAST<::jxx::lang::Object>(thisPtr())));
    }

    void AbstractTableModel::fireTableStructureChanged()
    {
        fireTableChanged(::jxx::NEW<
            ::jxx::swing::event::TableModelEvent>(
                ::jxx::CAST<::jxx::lang::Object>(thisPtr()),
                ::jxx::swing::event::TableModelEvent::HEADER_ROW));
    }

    void AbstractTableModel::fireTableRowsInserted(
        ::jxx::lang::jint firstRow, ::jxx::lang::jint lastRow)
    {
        fireTableChanged(::jxx::NEW<
            ::jxx::swing::event::TableModelEvent>(
                ::jxx::CAST<::jxx::lang::Object>(thisPtr()),
                firstRow, lastRow,
                ::jxx::swing::event::TableModelEvent::ALL_COLUMNS,
                ::jxx::swing::event::TableModelEvent::INSERT));
    }

    void AbstractTableModel::fireTableRowsUpdated(
        ::jxx::lang::jint firstRow, ::jxx::lang::jint lastRow)
    {
        fireTableChanged(::jxx::NEW<
            ::jxx::swing::event::TableModelEvent>(
                ::jxx::CAST<::jxx::lang::Object>(thisPtr()),
                firstRow, lastRow));
    }

    void AbstractTableModel::fireTableRowsDeleted(
        ::jxx::lang::jint firstRow, ::jxx::lang::jint lastRow)
    {
        fireTableChanged(::jxx::NEW<
            ::jxx::swing::event::TableModelEvent>(
                ::jxx::CAST<::jxx::lang::Object>(thisPtr()),
                firstRow, lastRow,
                ::jxx::swing::event::TableModelEvent::ALL_COLUMNS,
                ::jxx::swing::event::TableModelEvent::DELETE));
    }

    void AbstractTableModel::fireTableCellUpdated(
        ::jxx::lang::jint row, ::jxx::lang::jint column)
    {
        fireTableChanged(::jxx::NEW<
            ::jxx::swing::event::TableModelEvent>(
                ::jxx::CAST<::jxx::lang::Object>(thisPtr()),
                row, row, column,
                ::jxx::swing::event::TableModelEvent::UPDATE));
    }

    void AbstractTableModel::fireTableChanged(
        const ::jxx::Ptr<::jxx::swing::event::TableModelEvent>& event)
    {
        const auto listeners = listeners_;
        for (const auto& listener : listeners)
            if (listener != nullptr) listener->tableChanged(event);
    }

    void AbstractTableModel::writeObject(
        const ::jxx::Ptr<::jxx::io::ObjectOutputStream>&)
    {
    }

    void AbstractTableModel::readObject(
        const ::jxx::Ptr<::jxx::io::ObjectInputStream>&)
    {
        listeners_.clear();
    }

    void AbstractTableModel::readObjectNoData()
    {
        listeners_.clear();
    }
}
