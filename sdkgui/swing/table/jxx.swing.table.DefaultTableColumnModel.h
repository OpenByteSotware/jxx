#pragma once

#include <vector>

#include "io/jxx.io.SerializableI.h"
#include "swing/table/jxx.swing.table.TableColumnModel.h"

namespace jxx::swing::table
{
    class DefaultTableColumnModel : public ::jxx::lang::ClassBase<
        DefaultTableColumnModel, ::jxx::lang::Object,
        TableColumnModel, ::jxx::io::SerializableI>
    {
    public:
        using JxxSuper = ::jxx::lang::Object;
        using Super = ::jxx::lang::ClassBase<
            DefaultTableColumnModel, JxxSuper,
            TableColumnModel, ::jxx::io::SerializableI>;

        DefaultTableColumnModel();
        ~DefaultTableColumnModel() override = default;

        void addColumn(const ::jxx::Ptr<TableColumn>& column) override;
        void removeColumn(const ::jxx::Ptr<TableColumn>& column) override;
        void moveColumn(
            ::jxx::lang::jint columnIndex,
            ::jxx::lang::jint newIndex) override;
        void setColumnMargin(::jxx::lang::jint margin) override;
        ::jxx::lang::jint getColumnCount() const override;
        ::jxx::Ptr<TableColumn> getColumn(
            ::jxx::lang::jint columnIndex) const override;
        ::jxx::lang::jint getColumnIndex(
            const ::jxx::Ptr<::jxx::lang::Object>& identifier) const override;
        ::jxx::lang::jint getColumnMargin() const override;
        ::jxx::lang::jint getColumnIndexAtX(
            ::jxx::lang::jint xPosition) const override;
        ::jxx::lang::jint getTotalColumnWidth() const override;
        void setColumnSelectionAllowed(
            ::jxx::lang::jbool allowed) override;
        ::jxx::lang::jbool getColumnSelectionAllowed() const override;
        ::jxx::Ptr<::jxx::swing::ListSelectionModel>
            getSelectionModel() const override;
        void setSelectionModel(
            const ::jxx::Ptr<::jxx::swing::ListSelectionModel>& model)
            override;
        void addColumnModelListener(
            const ::jxx::Ptr<::jxx::swing::event::TableColumnModelListener>&
                listener) override;
        void removeColumnModelListener(
            const ::jxx::Ptr<::jxx::swing::event::TableColumnModelListener>&
                listener) override;

        void writeObject(
            const ::jxx::Ptr<::jxx::io::ObjectOutputStream>& out) override;
        void readObject(
            const ::jxx::Ptr<::jxx::io::ObjectInputStream>& in) override;
        void readObjectNoData() override;

    private:
        void fireAdded(::jxx::lang::jint index);
        void fireRemoved(::jxx::lang::jint index);
        void fireMoved(::jxx::lang::jint from, ::jxx::lang::jint to);
        void fireMarginChanged();

        std::vector<::jxx::Ptr<TableColumn>> columns_;
        std::vector<::jxx::Ptr<
            ::jxx::swing::event::TableColumnModelListener>> listeners_;
        ::jxx::Ptr<::jxx::swing::ListSelectionModel> selectionModel_;
        ::jxx::lang::jint columnMargin_ = 1;
        ::jxx::lang::jbool columnSelectionAllowed_ = false;
    };
}
