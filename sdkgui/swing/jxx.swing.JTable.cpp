#include "swing/jxx.swing.JTable.h"

#include <algorithm>

#include "lang/jxx.lang.IllegalArgumentException.h"
#include "lang/jxx.lang.IndexOutOfBoundsException.h"
#include "swing/event/jxx.swing.event.RowSorterEvent.h"
#include "swing/event/jxx.swing.event.TableModelEvent.h"
#include "swing/jxx.swing.RowSorter.h"
#include "swing/table/jxx.swing.table.DefaultTableCellRenderer.h"
#include "swing/table/jxx.swing.table.DefaultTableColumnModel.h"
#include "swing/table/jxx.swing.table.JTableHeader.h"
#include "swing/table/jxx.swing.table.TableCellEditor.h"
#include "swing/table/jxx.swing.table.TableCellRenderer.h"
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
        : Super(),
          gridColor_(::jxx::awt::Color::gray),
          intercellSpacing_(::jxx::NEW<::jxx::awt::Dimension>(1, 1)),
          defaultRenderer_(::jxx::NEW<
              ::jxx::swing::table::DefaultTableCellRenderer>())
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
        anchorRow_ = end;
    }

    void JTable::setColumnSelectionInterval(
        ::jxx::lang::jint start,
        ::jxx::lang::jint end)
    {
        if (start < 0 || end < 0
            || start >= getColumnCount() || end >= getColumnCount())
            throw ::jxx::lang::IndexOutOfBoundsException("column");
        selectedColumn_ = start;
        anchorColumn_ = end;
    }

    void JTable::changeSelection(
        ::jxx::lang::jint rowIndex,
        ::jxx::lang::jint columnIndex,
        ::jxx::lang::jbool toggle,
        ::jxx::lang::jbool extend)
    {
        if (rowIndex < 0 || rowIndex >= getRowCount()
            || columnIndex < 0 || columnIndex >= getColumnCount())
            throw ::jxx::lang::IndexOutOfBoundsException("cell");

        if (toggle && !extend
            && selectedRow_ == rowIndex
            && selectedColumn_ == columnIndex)
        {
            clearSelection();
            return;
        }

        if (!extend || anchorRow_ < 0 || anchorColumn_ < 0)
        {
            anchorRow_ = rowIndex;
            anchorColumn_ = columnIndex;
        }
        selectedRow_ = rowIndex;
        selectedColumn_ = columnIndex;
    }

    void JTable::selectAll()
    {
        if (getRowCount() == 0 || getColumnCount() == 0)
        {
            clearSelection();
            return;
        }
        anchorRow_ = 0;
        anchorColumn_ = 0;
        selectedRow_ = getRowCount() - 1;
        selectedColumn_ = getColumnCount() - 1;
    }

    void JTable::clearSelection()
    {
        selectedRow_ = -1;
        selectedColumn_ = -1;
        anchorRow_ = -1;
        anchorColumn_ = -1;
    }

    ::jxx::lang::jint JTable::getSelectedRow() const
    { return selectedRow_; }

    ::jxx::lang::jint JTable::getSelectedColumn() const
    { return selectedColumn_; }

    ::jxx::lang::jbool JTable::isRowSelected(::jxx::lang::jint row) const
    { return rowSelectionAllowed_ && selectedRow_ == row; }

    ::jxx::lang::jbool JTable::isColumnSelected(
        ::jxx::lang::jint column) const
    { return columnSelectionAllowed_ && selectedColumn_ == column; }

    void JTable::setRowSelectionAllowed(::jxx::lang::jbool allowed)
    { rowSelectionAllowed_ = allowed; }

    ::jxx::lang::jbool JTable::getRowSelectionAllowed() const
    { return rowSelectionAllowed_; }

    void JTable::setColumnSelectionAllowed(::jxx::lang::jbool allowed)
    {
        columnSelectionAllowed_ = allowed;
        if (columnModel_ != nullptr)
            columnModel_->setColumnSelectionAllowed(allowed);
    }

    ::jxx::lang::jbool JTable::getColumnSelectionAllowed() const
    { return columnSelectionAllowed_; }

    ::jxx::lang::jint JTable::rowAtPoint(
        const ::jxx::Ptr<::jxx::awt::Point>& point) const
    {
        if (point == nullptr || point->y < 0) return -1;
        const auto row = point->y / rowHeight_;
        return row < getRowCount() ? row : -1;
    }

    ::jxx::lang::jint JTable::columnAtPoint(
        const ::jxx::Ptr<::jxx::awt::Point>& point) const
    {
        return point == nullptr || columnModel_ == nullptr
            ? -1 : columnModel_->getColumnIndexAtX(point->x);
    }

    ::jxx::Ptr<::jxx::awt::Rectangle> JTable::getCellRect(
        ::jxx::lang::jint row,
        ::jxx::lang::jint column,
        ::jxx::lang::jbool includeSpacing) const
    {
        if (row < 0 || row >= getRowCount()
            || column < 0 || column >= getColumnCount())
            return ::jxx::NEW<::jxx::awt::Rectangle>();
        ::jxx::lang::jint x = 0;
        for (::jxx::lang::jint index = 0; index < column; ++index)
            x += columnModel_->getColumn(index)->getWidth();
        auto width = columnModel_->getColumn(column)->getWidth();
        auto height = rowHeight_;
        if (!includeSpacing && intercellSpacing_ != nullptr)
        {
            x += intercellSpacing_->width / 2;
            width = std::max<::jxx::lang::jint>(
                0, width - intercellSpacing_->width);
            height = std::max<::jxx::lang::jint>(
                0, height - intercellSpacing_->height);
        }
        return ::jxx::NEW<::jxx::awt::Rectangle>(
            x, row * rowHeight_, width, height);
    }

    void JTable::setGridColor(
        const ::jxx::Ptr<::jxx::awt::Color>& color)
    {
        if (color == nullptr)
            throw ::jxx::lang::IllegalArgumentException("color");
        gridColor_ = color;
    }

    ::jxx::Ptr<::jxx::awt::Color> JTable::getGridColor() const
    { return gridColor_; }

    void JTable::setShowHorizontalLines(::jxx::lang::jbool show)
    { showHorizontalLines_ = show; }
    ::jxx::lang::jbool JTable::getShowHorizontalLines() const
    { return showHorizontalLines_; }
    void JTable::setShowVerticalLines(::jxx::lang::jbool show)
    { showVerticalLines_ = show; }
    ::jxx::lang::jbool JTable::getShowVerticalLines() const
    { return showVerticalLines_; }
    void JTable::setShowGrid(::jxx::lang::jbool show)
    {
        showHorizontalLines_ = show;
        showVerticalLines_ = show;
    }

    void JTable::setIntercellSpacing(
        const ::jxx::Ptr<::jxx::awt::Dimension>& spacing)
    {
        if (spacing == nullptr
            || spacing->width < 0 || spacing->height < 0)
            throw ::jxx::lang::IllegalArgumentException("spacing");
        intercellSpacing_ = ::jxx::NEW<::jxx::awt::Dimension>(spacing);
        if (columnModel_ != nullptr)
            columnModel_->setColumnMargin(spacing->width);
    }

    ::jxx::Ptr<::jxx::awt::Dimension> JTable::getIntercellSpacing() const
    {
        return intercellSpacing_ == nullptr ? nullptr
            : ::jxx::NEW<::jxx::awt::Dimension>(intercellSpacing_);
    }

    ::jxx::Ptr<::jxx::swing::table::TableCellRenderer>
    JTable::getCellRenderer(
        ::jxx::lang::jint row,
        ::jxx::lang::jint column) const
    {
        if (row < 0 || row >= getRowCount()
            || column < 0 || column >= getColumnCount())
            throw ::jxx::lang::IndexOutOfBoundsException("cell");
        const auto renderer = columnModel_->getColumn(column)->getCellRenderer();
        return renderer == nullptr ? defaultRenderer_ : renderer;
    }

    ::jxx::Ptr<::jxx::awt::Component> JTable::prepareRenderer(
        const ::jxx::Ptr<::jxx::swing::table::TableCellRenderer>& renderer,
        ::jxx::lang::jint row,
        ::jxx::lang::jint column)
    {
        if (renderer == nullptr)
            throw ::jxx::lang::IllegalArgumentException("renderer");
        return renderer->getTableCellRendererComponent(
            ::jxx::CAST<JTable>(thisPtr()), getValueAt(row, column),
            isRowSelected(row) || isColumnSelected(column),
            false, row, column);
    }

    void JTable::setDefaultRenderer(
        const ::jxx::Ptr<::jxx::swing::table::TableCellRenderer>& renderer)
    { defaultRenderer_ = renderer; }

    ::jxx::Ptr<::jxx::swing::table::TableCellRenderer>
    JTable::getDefaultRenderer() const
    { return defaultRenderer_; }

    ::jxx::Ptr<::jxx::swing::table::TableCellEditor>
    JTable::getCellEditor(
        ::jxx::lang::jint row,
        ::jxx::lang::jint column) const
    {
        if (row < 0 || row >= getRowCount()
            || column < 0 || column >= getColumnCount())
            throw ::jxx::lang::IndexOutOfBoundsException("cell");
        return columnModel_->getColumn(column)->getCellEditor();
    }

    ::jxx::Ptr<::jxx::awt::Component> JTable::prepareEditor(
        const ::jxx::Ptr<::jxx::swing::table::TableCellEditor>& editor,
        ::jxx::lang::jint row,
        ::jxx::lang::jint column)
    {
        if (editor == nullptr)
            throw ::jxx::lang::IllegalArgumentException("editor");
        return editor->getTableCellEditorComponent(
            ::jxx::CAST<JTable>(thisPtr()), getValueAt(row, column),
            true, row, column);
    }

    void JTable::setCellEditor(
        const ::jxx::Ptr<::jxx::swing::table::TableCellEditor>& editor)
    { cellEditor_ = editor; }

    ::jxx::Ptr<::jxx::swing::table::TableCellEditor>
    JTable::getCellEditor() const
    { return cellEditor_; }

    ::jxx::lang::jbool JTable::editCellAt(
        ::jxx::lang::jint row,
        ::jxx::lang::jint column)
    {
        const auto editor = getCellEditor(row, column);
        if (editor == nullptr || !model_->isCellEditable(
                convertRowIndexToModel(row),
                convertColumnIndexToModel(column)))
            return false;
        removeEditor();
        cellEditor_ = editor;
        editingRow_ = row;
        editingColumn_ = column;
        prepareEditor(editor, row, column);
        return true;
    }

    ::jxx::lang::jbool JTable::isEditing() const
    { return cellEditor_ != nullptr; }
    ::jxx::lang::jint JTable::getEditingRow() const
    { return editingRow_; }
    ::jxx::lang::jint JTable::getEditingColumn() const
    { return editingColumn_; }

    void JTable::removeEditor()
    {
        cellEditor_.reset();
        editingRow_ = -1;
        editingColumn_ = -1;
    }

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
