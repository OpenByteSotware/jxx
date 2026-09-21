#pragma once

#include "io/jxx.io.SerializableI.h"
#include "swing/jxx.swing.JComponent.h"
#include "swing/event/jxx.swing.event.TableColumnModelListener.h"

namespace jxx::awt
{
    class Point;
}

namespace jxx::swing
{
    class JTable;
}

namespace jxx::swing::table
{
    class TableCellRenderer;
    class TableColumn;
    class TableColumnModel;

    class JTableHeader : public ::jxx::lang::ClassBase<
        JTableHeader, ::jxx::swing::JComponent,
        ::jxx::swing::event::TableColumnModelListener,
        ::jxx::io::SerializableI>
    {
    public:
        using JxxSuper = ::jxx::swing::JComponent;
        using Super = ::jxx::lang::ClassBase<
            JTableHeader, JxxSuper,
            ::jxx::swing::event::TableColumnModelListener,
            ::jxx::io::SerializableI>;

        JTableHeader();
        explicit JTableHeader(const ::jxx::Ptr<TableColumnModel>& model);
        ~JTableHeader() override = default;

        void setTable(const ::jxx::Ptr<::jxx::swing::JTable>& table);
        ::jxx::Ptr<::jxx::swing::JTable> getTable() const;
        void setColumnModel(const ::jxx::Ptr<TableColumnModel>& model);
        ::jxx::Ptr<TableColumnModel> getColumnModel() const;
        void setReorderingAllowed(::jxx::lang::jbool allowed);
        ::jxx::lang::jbool getReorderingAllowed() const;
        void setResizingAllowed(::jxx::lang::jbool allowed);
        ::jxx::lang::jbool getResizingAllowed() const;
        void setDraggedColumn(const ::jxx::Ptr<TableColumn>& column);
        ::jxx::Ptr<TableColumn> getDraggedColumn() const;
        void setDraggedDistance(::jxx::lang::jint distance);
        ::jxx::lang::jint getDraggedDistance() const;
        void setResizingColumn(const ::jxx::Ptr<TableColumn>& column);
        ::jxx::Ptr<TableColumn> getResizingColumn() const;
        void setDefaultRenderer(
            const ::jxx::Ptr<TableCellRenderer>& renderer);
        ::jxx::Ptr<TableCellRenderer> getDefaultRenderer() const;
        ::jxx::lang::jint columnAtPoint(
            const ::jxx::Ptr<::jxx::awt::Point>& point) const;
        void resizeAndRepaint();

        void columnAdded(const ::jxx::Ptr<
            ::jxx::swing::event::TableColumnModelEvent>& event) override;
        void columnRemoved(const ::jxx::Ptr<
            ::jxx::swing::event::TableColumnModelEvent>& event) override;
        void columnMoved(const ::jxx::Ptr<
            ::jxx::swing::event::TableColumnModelEvent>& event) override;
        void columnMarginChanged(const ::jxx::Ptr<
            ::jxx::swing::event::ChangeEvent>& event) override;
        void columnSelectionChanged(const ::jxx::Ptr<
            ::jxx::swing::event::ListSelectionEvent>& event) override;

        void writeObject(
            const ::jxx::Ptr<::jxx::io::ObjectOutputStream>& out) override;
        void readObject(
            const ::jxx::Ptr<::jxx::io::ObjectInputStream>& in) override;
        void readObjectNoData() override;

    private:
        ::jxx::Ptr<TableColumnModel> columnModel_;
        ::jxx::Ptr<::jxx::swing::JTable> table_;
        ::jxx::Ptr<TableColumn> draggedColumn_;
        ::jxx::Ptr<TableColumn> resizingColumn_;
        ::jxx::Ptr<TableCellRenderer> defaultRenderer_;
        ::jxx::lang::jint draggedDistance_ = 0;
        ::jxx::lang::jbool reorderingAllowed_ = true;
        ::jxx::lang::jbool resizingAllowed_ = true;
    };
}
