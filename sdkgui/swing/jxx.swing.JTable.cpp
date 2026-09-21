#include "swing/jxx.swing.JTable.h"

#include "lang/jxx.lang.IllegalArgumentException.h"
#include "lang/jxx.lang.IndexOutOfBoundsException.h"
#include "swing/event/jxx.swing.event.RowSorterEvent.h"
#include "swing/event/jxx.swing.event.TableModelEvent.h"
#include "swing/jxx.swing.RowSorter.h"
#include "swing/table/jxx.swing.table.DefaultTableColumnModel.h"
#include "swing/table/jxx.swing.table.JTableHeader.h"
#include "swing/table/jxx.swing.table.TableColumn.h"
#include "swing/table/jxx.swing.table.TableColumnModel.h"
#include "swing/table/jxx.swing.table.TableModel.h"
#include "swing/table/jxx.swing.table.TableRowSorter.h"

namespace jxx::swing
{
    JTable::JTable()
        : JTable(::jxx::NEW<::jxx::swing::table::DefaultTableModel>())
    {
    }

    JTable::JTable(
        const ::jxx::Ptr<::jxx::swing::table::TableModel>& model)
        : JTable(model,
            ::jxx::NEW<
                ::jxx::swing::table::DefaultTableColumnModel>())
    {
    }

    JTable::JTable(
        ::jxx::lang::jint rows,
        ::jxx::lang::jint columns)
        : JTable(::jxx::NEW<::jxx::swing::table::DefaultTableModel>(
            rows, columns))
    {
    }

    JTable::JTable(
        const ::jxx::Ptr<::jxx::swing::table::TableModel>& model,
        const ::jxx::Ptr<::jxx::swing::table::TableColumnModel>&
            columnModel)
        : Super()
    {
        setColumnModel(columnModel);
        setModel(model);
        setTableHeader(
            ::jxx::NEW<::jxx::swing::table::JTableHeader>(columnModel_));
    }

    void JTable::setModel(
        const ::jxx::Ptr<::jxx::swing::table::TableModel>& model)
    {
        if (model == nullptr)
            throw ::jxx::lang::NullPointerException("model");
        const auto listener = ::jxx::CAST<
            ::jxx::swing::event::TableModelListener>(thisPtr());
        if (model_ != nullptr) model_->removeTableModelListener(listener);
        model_ = model;
        model_->addTableModelListener(listener);
        clearSelection();
        createDefaultColumnsFromModel();
        if (autoCreateRowSorter_)
        {
            setRowSorter(::jxx::NEW<
                ::jxx::swing::table::TableRowSorter<
                    ::jxx::swing::table::TableModel>>(model_));
        }
    }

    ::jxx::Ptr<::jxx::swing::table::TableModel>
    JTable::getModel() const
    {
        return model_;
    }

    void JTable::setColumnModel(
        const ::jxx::Ptr<::jxx::swing::table::TableColumnModel>& model)
    {
        if (model == nullptr)
            throw ::jxx::lang::NullPointerException("columnModel");
        const auto listener = ::jxx::CAST<
            ::jxx::swing::event::TableColumnModelListener>(thisPtr());
        if (columnModel_ != nullptr)
            columnModel_->removeColumnModelListener(listener);
        columnModel_ = model;
        columnModel_->addColumnModelListener(listener);
        if (tableHeader_ != nullptr) tableHeader_->setColumnModel(model);
        invalidate();
    }

    ::jxx::Ptr<::jxx::swing::table::TableColumnModel>
    JTable::getColumnModel() const
    {
        return columnModel_;
    }

    void JTable::setTableHeader(
        const ::jxx::Ptr<::jxx::swing::table::JTableHeader>& header)
    {
        tableHeader_ = header;
        if (tableHeader_ != nullptr)
        {
            tableHeader_->setTable(::jxx::CAST<JTable>(thisPtr()));
            tableHeader_->setColumnModel(columnModel_);
        }
    }

    ::jxx::Ptr<::jxx::swing::table::JTableHeader>
    JTable::getTableHeader() const
    {
        return tableHeader_;
    }

    void JTable::setRowSorter(
        const ::jxx::Ptr<RowSorter<
            ::jxx::swing::table::TableModel>>& sorter)
    {
        const auto listener = ::jxx::CAST<
            ::jxx::swing::event::RowSorterListener>(thisPtr());
        if (rowSorter_ != nullptr)
            rowSorter_->removeRowSorterListener(listener);
        rowSorter_ = sorter;
        if (rowSorter_ != nullptr) rowSorter_->addRowSorterListener(listener);
        clearSelection();
        invalidate();
    }

    ::jxx::Ptr<RowSorter<::jxx::swing::table::TableModel>>
    JTable::getRowSorter() const
    {
        return rowSorter_;
    }

    void JTable::setAutoCreateRowSorter(::jxx::lang::jbool value)
    {
        autoCreateRowSorter_ = value;
        if (value && model_ != nullptr)
        {
            setRowSorter(::jxx::NEW<
                ::jxx::swing::table::TableRowSorter<
                    ::jxx::swing::table::TableModel>>(model_));
        }
    }

    ::jxx::lang::jbool JTable::getAutoCreateRowSorter() const
    {
        return autoCreateRowSorter_;
    }

    ::jxx::lang::jint JTable::convertRowIndexToModel(
        ::jxx::lang::jint viewRowIndex) const
    {
        if (viewRowIndex < 0 || viewRowIndex >= getRowCount())
            throw ::jxx::lang::IndexOutOfBoundsException("row");
        return rowSorter_ == nullptr
            ? viewRowIndex
            : rowSorter_->convertRowIndexToModel(viewRowIndex);
    }

    ::jxx::lang::jint JTable::convertRowIndexToView(
        ::jxx::lang::jint modelRowIndex) const
    {
        if (modelRowIndex < 0
            || model_ == nullptr
            || modelRowIndex >= model_->getRowCount())
            throw ::jxx::lang::IndexOutOfBoundsException("row");
        return rowSorter_ == nullptr
            ? modelRowIndex
            : rowSorter_->convertRowIndexToView(modelRowIndex);
    }

    void JTable::addColumn(
        const ::jxx::Ptr<::jxx::swing::table::TableColumn>& column)
    {
        columnModel_->addColumn(column);
    }

    void JTable::removeColumn(
        const ::jxx::Ptr<::jxx::swing::table::TableColumn>& column)
    {
        columnModel_->removeColumn(column);
    }

    void JTable::moveColumn(
        ::jxx::lang::jint column,
        ::jxx::lang::jint targetColumn)
    {
        columnModel_->moveColumn(column, targetColumn);
    }

    ::jxx::Ptr<::jxx::swing::table::TableColumn> JTable::getColumn(
        const ::jxx::Ptr<::jxx::lang::Object>& identifier) const
    {
        return columnModel_->getColumn(
            columnModel_->getColumnIndex(identifier));
    }

    ::jxx::lang::jint JTable::convertColumnIndexToModel(
        ::jxx::lang::jint viewColumnIndex) const
    {
        if (viewColumnIndex < 0) return viewColumnIndex;
        return columnModel_->getColumn(viewColumnIndex)->getModelIndex();
    }

    ::jxx::lang::jint JTable::convertColumnIndexToView(
        ::jxx::lang::jint modelColumnIndex) const
    {
        if (modelColumnIndex < 0) return modelColumnIndex;
        for (::jxx::lang::jint index = 0;
             index < columnModel_->getColumnCount(); ++index)
        {
            if (columnModel_->getColumn(index)->getModelIndex()
                == modelColumnIndex)
                return index;
        }
        return -1;
    }

    ::jxx::lang::jint JTable::getRowCount() const
    {
        return rowSorter_ == nullptr
            ? model_->getRowCount()
            : rowSorter_->getViewRowCount();
    }

    ::jxx::lang::jint JTable::getColumnCount() const
    {
        return columnModel_ == nullptr
            ? 0 : columnModel_->getColumnCount();
    }

    ::jxx::Ptr<::jxx::lang::Object> JTable::getValueAt(
        ::jxx::lang::jint row,
        ::jxx::lang::jint column) const
    {
        return model_->getValueAt(
            convertRowIndexToModel(row),
            convertColumnIndexToModel(column));
    }

    void JTable::setValueAt(
        const ::jxx::Ptr<::jxx::lang::Object>& value,
        ::jxx::lang::jint row,
        ::jxx::lang::jint column)
    {
        model_->setValueAt(value,
            convertRowIndexToModel(row),
            convertColumnIndexToModel(column));
    }

    void JTable::setRowSelectionInterval(
        ::jxx::lang::jint start,
        ::jxx::lang::jint end)
    {
        if (start < 0 || end < 0
            || start >= getRowCount() || end >= getRowCount())
            throw ::jxx::lang::IndexOutOfBoundsException("row");
        selectedRow_ = start;
    }

    void JTable::clearSelection() { selectedRow_ = -1; }
    ::jxx::lang::jint JTable::getSelectedRow() const
    { return selectedRow_; }

    void JTable::setRowHeight(::jxx::lang::jint height)
    {
        if (height <= 0)
            throw ::jxx::lang::IllegalArgumentException("height");
        rowHeight_ = height;
    }

    ::jxx::lang::jint JTable::getRowHeight() const { return rowHeight_; }

    void JTable::setAutoResizeMode(::jxx::lang::jint mode)
    {
        if (mode < AUTO_RESIZE_OFF || mode > AUTO_RESIZE_ALL_COLUMNS)
            throw ::jxx::lang::IllegalArgumentException("mode");
        autoResizeMode_ = mode;
    }

    ::jxx::lang::jint JTable::getAutoResizeMode() const
    { return autoResizeMode_; }

    ::jxx::Ptr<::jxx::awt::Dimension>
    JTable::getPreferredScrollableViewportSize() const
    {
        return ::jxx::NEW<::jxx::awt::Dimension>(
            columnModel_ == nullptr
                ? getColumnCount() * 75
                : columnModel_->getTotalColumnWidth(),
            getRowCount() * rowHeight_);
    }

    void JTable::createDefaultColumnsFromModel()
    {
        if (model_ == nullptr || columnModel_ == nullptr) return;
        while (columnModel_->getColumnCount() > 0)
            columnModel_->removeColumn(columnModel_->getColumn(0));
        for (::jxx::lang::jint index = 0;
             index < model_->getColumnCount(); ++index)
        {
            auto column = ::jxx::NEW<
                ::jxx::swing::table::TableColumn>(index);
            column->setHeaderValue(::jxx::CAST<::jxx::lang::Object>(
                model_->getColumnName(index)));
            columnModel_->addColumn(column);
        }
    }

    void JTable::tableChanged(
        const ::jxx::Ptr<::jxx::swing::event::TableModelEvent>& event)
    {
        if (event == nullptr) return;
        if (event->getFirstRow()
            == ::jxx::swing::event::TableModelEvent::HEADER_ROW)
        {
            createDefaultColumnsFromModel();
            if (rowSorter_ != nullptr)
                rowSorter_->modelStructureChanged();
        }
        else if (rowSorter_ != nullptr)
        {
            if (event->getType()
                == ::jxx::swing::event::TableModelEvent::INSERT)
                rowSorter_->rowsInserted(
                    event->getFirstRow(), event->getLastRow());
            else if (event->getType()
                == ::jxx::swing::event::TableModelEvent::DELETE)
                rowSorter_->rowsDeleted(
                    event->getFirstRow(), event->getLastRow());
            else if (event->getColumn()
                == ::jxx::swing::event::TableModelEvent::ALL_COLUMNS)
                rowSorter_->rowsUpdated(
                    event->getFirstRow(), event->getLastRow());
            else
                rowSorter_->rowsUpdated(event->getFirstRow(),
                    event->getLastRow(), event->getColumn());
        }
        clearSelection();
        invalidate();
    }

    void JTable::sorterChanged(
        const ::jxx::Ptr<::jxx::swing::event::RowSorterEvent>&)
    { clearSelection(); invalidate(); }
    void JTable::columnAdded(const ::jxx::Ptr<
        ::jxx::swing::event::TableColumnModelEvent>&)
    { invalidate(); }
    void JTable::columnRemoved(const ::jxx::Ptr<
        ::jxx::swing::event::TableColumnModelEvent>&)
    { invalidate(); }
    void JTable::columnMoved(const ::jxx::Ptr<
        ::jxx::swing::event::TableColumnModelEvent>&)
    { invalidate(); }
    void JTable::columnMarginChanged(const ::jxx::Ptr<
        ::jxx::swing::event::ChangeEvent>&)
    { invalidate(); }
    void JTable::columnSelectionChanged(const ::jxx::Ptr<
        ::jxx::swing::event::ListSelectionEvent>&)
    { invalidate(); }
}
