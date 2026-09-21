#pragma once

#include "lang/jxx.lang.ClassInfo.h"

namespace jxx::swing
{
    class ListSelectionModel;
}

namespace jxx::swing::event
{
    class TableColumnModelListener;
}

namespace jxx::swing::table
{
    class TableColumn;

    class TableColumnModel : public ::jxx::lang::InterfaceBase<
        TableColumnModel>
    {
    public:
        ~TableColumnModel() override = default;

        virtual void addColumn(const ::jxx::Ptr<TableColumn>& column) = 0;
        virtual void removeColumn(const ::jxx::Ptr<TableColumn>& column) = 0;
        virtual void moveColumn(
            ::jxx::lang::jint columnIndex,
            ::jxx::lang::jint newIndex) = 0;
        virtual void setColumnMargin(::jxx::lang::jint margin) = 0;
        virtual ::jxx::lang::jint getColumnCount() const = 0;
        virtual ::jxx::Ptr<TableColumn> getColumn(
            ::jxx::lang::jint columnIndex) const = 0;
        virtual ::jxx::lang::jint getColumnIndex(
            const ::jxx::Ptr<::jxx::lang::Object>& identifier) const = 0;
        virtual ::jxx::lang::jint getColumnMargin() const = 0;
        virtual ::jxx::lang::jint getColumnIndexAtX(
            ::jxx::lang::jint xPosition) const = 0;
        virtual ::jxx::lang::jint getTotalColumnWidth() const = 0;
        virtual void setColumnSelectionAllowed(
            ::jxx::lang::jbool allowed) = 0;
        virtual ::jxx::lang::jbool getColumnSelectionAllowed() const = 0;
        virtual ::jxx::Ptr<::jxx::swing::ListSelectionModel>
            getSelectionModel() const = 0;
        virtual void setSelectionModel(
            const ::jxx::Ptr<::jxx::swing::ListSelectionModel>& model) = 0;
        virtual void addColumnModelListener(
            const ::jxx::Ptr<::jxx::swing::event::TableColumnModelListener>&
                listener) = 0;
        virtual void removeColumnModelListener(
            const ::jxx::Ptr<::jxx::swing::event::TableColumnModelListener>&
                listener) = 0;
    };
}
