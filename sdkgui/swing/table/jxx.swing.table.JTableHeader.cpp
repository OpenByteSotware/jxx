#include "swing/table/jxx.swing.table.JTableHeader.h"

#include "awt/jxx.awt.Point.h"
#include "lang/jxx.lang.IllegalArgumentException.h"
#include "swing/jxx.swing.JTable.h"
#include "swing/table/jxx.swing.table.DefaultTableCellRenderer.h"
#include "swing/table/jxx.swing.table.DefaultTableColumnModel.h"
#include "swing/table/jxx.swing.table.TableColumn.h"
#include "swing/table/jxx.swing.table.TableColumnModel.h"

namespace jxx::swing::table
{
    JTableHeader::JTableHeader()
        : JTableHeader(::jxx::NEW<DefaultTableColumnModel>())
    {
    }

    JTableHeader::JTableHeader(
        const ::jxx::Ptr<TableColumnModel>& model)
        : Super(),
          defaultRenderer_(::jxx::NEW<DefaultTableCellRenderer>())
    {
        setColumnModel(model);
    }

    void JTableHeader::setTable(
        const ::jxx::Ptr<::jxx::swing::JTable>& table)
    {
        table_ = table;
    }

    ::jxx::Ptr<::jxx::swing::JTable> JTableHeader::getTable() const
    {
        return table_;
    }

    void JTableHeader::setColumnModel(
        const ::jxx::Ptr<TableColumnModel>& model)
    {
        if (model == nullptr)
            throw ::jxx::lang::IllegalArgumentException("model");
        if (columnModel_ != nullptr)
            columnModel_->removeColumnModelListener(
                ::jxx::CAST<::jxx::swing::event::TableColumnModelListener>(
                    thisPtr()));
        columnModel_ = model;
        columnModel_->addColumnModelListener(
            ::jxx::CAST<::jxx::swing::event::TableColumnModelListener>(
                thisPtr()));
        resizeAndRepaint();
    }

    ::jxx::Ptr<TableColumnModel> JTableHeader::getColumnModel() const
    {
        return columnModel_;
    }

    void JTableHeader::setReorderingAllowed(::jxx::lang::jbool allowed)
    { reorderingAllowed_ = allowed; }
    ::jxx::lang::jbool JTableHeader::getReorderingAllowed() const
    { return reorderingAllowed_; }
    void JTableHeader::setResizingAllowed(::jxx::lang::jbool allowed)
    { resizingAllowed_ = allowed; }
    ::jxx::lang::jbool JTableHeader::getResizingAllowed() const
    { return resizingAllowed_; }
    void JTableHeader::setDraggedColumn(
        const ::jxx::Ptr<TableColumn>& column)
    { draggedColumn_ = column; }
    ::jxx::Ptr<TableColumn> JTableHeader::getDraggedColumn() const
    { return draggedColumn_; }
    void JTableHeader::setDraggedDistance(::jxx::lang::jint distance)
    { draggedDistance_ = distance; }
    ::jxx::lang::jint JTableHeader::getDraggedDistance() const
    { return draggedDistance_; }
    void JTableHeader::setResizingColumn(
        const ::jxx::Ptr<TableColumn>& column)
    { resizingColumn_ = column; }
    ::jxx::Ptr<TableColumn> JTableHeader::getResizingColumn() const
    { return resizingColumn_; }
    void JTableHeader::setDefaultRenderer(
        const ::jxx::Ptr<TableCellRenderer>& renderer)
    { defaultRenderer_ = renderer; }
    ::jxx::Ptr<TableCellRenderer> JTableHeader::getDefaultRenderer() const
    { return defaultRenderer_; }

    ::jxx::lang::jint JTableHeader::columnAtPoint(
        const ::jxx::Ptr<::jxx::awt::Point>& point) const
    {
        return point == nullptr || columnModel_ == nullptr
            ? -1 : columnModel_->getColumnIndexAtX(point->x);
    }

    void JTableHeader::resizeAndRepaint()
    {
        invalidate();
    }

    void JTableHeader::columnAdded(const ::jxx::Ptr<
        ::jxx::swing::event::TableColumnModelEvent>&)
    { resizeAndRepaint(); }
    void JTableHeader::columnRemoved(const ::jxx::Ptr<
        ::jxx::swing::event::TableColumnModelEvent>&)
    { resizeAndRepaint(); }
    void JTableHeader::columnMoved(const ::jxx::Ptr<
        ::jxx::swing::event::TableColumnModelEvent>&)
    { resizeAndRepaint(); }
    void JTableHeader::columnMarginChanged(const ::jxx::Ptr<
        ::jxx::swing::event::ChangeEvent>&)
    { resizeAndRepaint(); }
    void JTableHeader::columnSelectionChanged(const ::jxx::Ptr<
        ::jxx::swing::event::ListSelectionEvent>&)
    { invalidate(); }

    void JTableHeader::writeObject(
        const ::jxx::Ptr<::jxx::io::ObjectOutputStream>&)
    {
    }

    void JTableHeader::readObject(
        const ::jxx::Ptr<::jxx::io::ObjectInputStream>&)
    {
        draggedColumn_.reset();
        resizingColumn_.reset();
        draggedDistance_ = 0;
    }

    void JTableHeader::readObjectNoData()
    {
        columnModel_ = ::jxx::NEW<DefaultTableColumnModel>();
        defaultRenderer_ = ::jxx::NEW<DefaultTableCellRenderer>();
        table_.reset();
        draggedColumn_.reset();
        resizingColumn_.reset();
        draggedDistance_ = 0;
        reorderingAllowed_ = true;
        resizingAllowed_ = true;
    }
}
