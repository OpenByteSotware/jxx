#pragma once

#include "awt/jxx.awt.Dimension.h"
#include "lang/jxx.lang.NullPointerException.h"
#include "swing/jxx.swing.JComponent.h"
#include "swing/event/jxx.swing.event.RowSorterListener.h"
#include "swing/event/jxx.swing.event.TableColumnModelListener.h"
#include "swing/event/jxx.swing.event.TableModelListener.h"
#include "swing/table/jxx.swing.table.DefaultTableModel.h"

namespace jxx::swing::event
{
    class ChangeEvent;
    class ListSelectionEvent;
    class RowSorterEvent;
    class TableColumnModelEvent;
    class TableModelEvent;
}

namespace jxx::swing::table
{
    class DefaultTableColumnModel;
    class JTableHeader;
    class TableColumn;
    class TableColumnModel;
    class TableModel;
}

namespace jxx::swing
{
    template<typename M>
    class RowSorter;

    class JTable : public ::jxx::lang::ClassBase<
        JTable, JComponent,
        ::jxx::swing::event::TableModelListener,
        ::jxx::swing::event::TableColumnModelListener,
        ::jxx::swing::event::RowSorterListener>
    {
    public:
        using JxxSuper = JComponent;
        using Super = ::jxx::lang::ClassBase<
            JTable, JxxSuper,
            ::jxx::swing::event::TableModelListener,
            ::jxx::swing::event::TableColumnModelListener,
            ::jxx::swing::event::RowSorterListener>;

        static constexpr ::jxx::lang::jint AUTO_RESIZE_OFF = 0;
        static constexpr ::jxx::lang::jint AUTO_RESIZE_NEXT_COLUMN = 1;
        static constexpr ::jxx::lang::jint AUTO_RESIZE_SUBSEQUENT_COLUMNS = 2;
        static constexpr ::jxx::lang::jint AUTO_RESIZE_LAST_COLUMN = 3;
        static constexpr ::jxx::lang::jint AUTO_RESIZE_ALL_COLUMNS = 4;

        JTable();
        explicit JTable(
            const ::jxx::Ptr<::jxx::swing::table::TableModel>& model);
        JTable(
            ::jxx::lang::jint rows,
            ::jxx::lang::jint columns);
        JTable(
            const ::jxx::Ptr<::jxx::swing::table::TableModel>& model,
            const ::jxx::Ptr<::jxx::swing::table::TableColumnModel>&
                columnModel);
        ~JTable() override = default;

        void setModel(
            const ::jxx::Ptr<::jxx::swing::table::TableModel>& model);
        ::jxx::Ptr<::jxx::swing::table::TableModel> getModel() const;
        void setColumnModel(
            const ::jxx::Ptr<::jxx::swing::table::TableColumnModel>& model);
        ::jxx::Ptr<::jxx::swing::table::TableColumnModel>
            getColumnModel() const;
        void setTableHeader(
            const ::jxx::Ptr<::jxx::swing::table::JTableHeader>& header);
        ::jxx::Ptr<::jxx::swing::table::JTableHeader>
            getTableHeader() const;

        void setRowSorter(
            const ::jxx::Ptr<RowSorter<
                ::jxx::swing::table::TableModel>>& sorter);
        ::jxx::Ptr<RowSorter<::jxx::swing::table::TableModel>>
            getRowSorter() const;
        void setAutoCreateRowSorter(::jxx::lang::jbool value);
        ::jxx::lang::jbool getAutoCreateRowSorter() const;
        ::jxx::lang::jint convertRowIndexToModel(
            ::jxx::lang::jint viewRowIndex) const;
        ::jxx::lang::jint convertRowIndexToView(
            ::jxx::lang::jint modelRowIndex) const;

        void addColumn(
            const ::jxx::Ptr<::jxx::swing::table::TableColumn>& column);
        void removeColumn(
            const ::jxx::Ptr<::jxx::swing::table::TableColumn>& column);
        void moveColumn(
            ::jxx::lang::jint column,
            ::jxx::lang::jint targetColumn);
        ::jxx::Ptr<::jxx::swing::table::TableColumn> getColumn(
            const ::jxx::Ptr<::jxx::lang::Object>& identifier) const;
        ::jxx::lang::jint convertColumnIndexToModel(
            ::jxx::lang::jint viewColumnIndex) const;
        ::jxx::lang::jint convertColumnIndexToView(
            ::jxx::lang::jint modelColumnIndex) const;

        ::jxx::lang::jint getRowCount() const;
        ::jxx::lang::jint getColumnCount() const;
        ::jxx::Ptr<::jxx::lang::Object> getValueAt(
            ::jxx::lang::jint row,
            ::jxx::lang::jint column) const;
        void setValueAt(
            const ::jxx::Ptr<::jxx::lang::Object>& value,
            ::jxx::lang::jint row,
            ::jxx::lang::jint column);
        void setRowSelectionInterval(
            ::jxx::lang::jint start,
            ::jxx::lang::jint end);
        void clearSelection();
        ::jxx::lang::jint getSelectedRow() const;
        void setRowHeight(::jxx::lang::jint height);
        ::jxx::lang::jint getRowHeight() const;
        void setAutoResizeMode(::jxx::lang::jint mode);
        ::jxx::lang::jint getAutoResizeMode() const;
        ::jxx::Ptr<::jxx::awt::Dimension>
            getPreferredScrollableViewportSize() const;

        void tableChanged(
            const ::jxx::Ptr<::jxx::swing::event::TableModelEvent>& event)
            override;
        void sorterChanged(
            const ::jxx::Ptr<::jxx::swing::event::RowSorterEvent>& event)
            override;
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

    private:
        void createDefaultColumnsFromModel();

        ::jxx::Ptr<::jxx::swing::table::TableModel> model_;
        ::jxx::Ptr<::jxx::swing::table::TableColumnModel> columnModel_;
        ::jxx::Ptr<::jxx::swing::table::JTableHeader> tableHeader_;
        ::jxx::Ptr<RowSorter<::jxx::swing::table::TableModel>> rowSorter_;
        ::jxx::lang::jint selectedRow_ = -1;
        ::jxx::lang::jint rowHeight_ = 16;
        ::jxx::lang::jint autoResizeMode_ = AUTO_RESIZE_SUBSEQUENT_COLUMNS;
        ::jxx::lang::jbool autoCreateRowSorter_ = false;
    };
}
