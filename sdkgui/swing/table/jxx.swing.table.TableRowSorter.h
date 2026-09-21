#pragma once

#include "lang/jxx.lang.Integer.h"
#include "swing/jxx.swing.DefaultRowSorter.h"
#include "swing/table/jxx.swing.table.TableModel.h"

namespace jxx::swing::table
{
    template<typename M = TableModel>
    class TableRowSorter : public ::jxx::lang::ClassBase<
        TableRowSorter<M>,
        ::jxx::swing::DefaultRowSorter<M, ::jxx::lang::Integer>>
    {
    public:
        using JxxSuper = ::jxx::swing::DefaultRowSorter<
            M, ::jxx::lang::Integer>;
        using Super = ::jxx::lang::ClassBase<TableRowSorter<M>, JxxSuper>;
        using ModelWrapper = typename JxxSuper::ModelWrapper;

        TableRowSorter() : Super()
        {
        }

        explicit TableRowSorter(const ::jxx::Ptr<M>& model) : Super()
        {
            setModel(model);
        }

        void setModel(const ::jxx::Ptr<M>& model)
        {
            model_ = model;
            if (model == nullptr)
            {
                this->setModelWrapper(nullptr);
                return;
            }
            this->setModelWrapper(::jxx::NEW<TableModelWrapper>(model));
        }

    private:
        class TableModelWrapper final : public ::jxx::lang::ClassBase<
            TableModelWrapper, ModelWrapper>
        {
        public:
            using JxxSuper = ModelWrapper;
            using Super = ::jxx::lang::ClassBase<
                TableModelWrapper, JxxSuper>;

            explicit TableModelWrapper(const ::jxx::Ptr<M>& model)
                : Super(), model_(model)
            {
            }

            ::jxx::Ptr<M> getModel() const override { return model_; }
            ::jxx::lang::jint getColumnCount() const override
            { return model_->getColumnCount(); }
            ::jxx::lang::jint getRowCount() const override
            { return model_->getRowCount(); }
            ::jxx::Ptr<::jxx::lang::Object> getValueAt(
                ::jxx::lang::jint row,
                ::jxx::lang::jint column) const override
            { return model_->getValueAt(row, column); }
            ::jxx::Ptr<::jxx::lang::Integer> getIdentifier(
                ::jxx::lang::jint row) const override
            { return ::jxx::NEW<::jxx::lang::Integer>(row); }

        private:
            ::jxx::Ptr<M> model_;
        };

        ::jxx::Ptr<M> model_;
    };
}
