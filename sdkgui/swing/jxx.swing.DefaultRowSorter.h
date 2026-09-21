#pragma once

#include <algorithm>
#include <numeric>
#include <vector>

#include "lang/jxx.lang.IndexOutOfBoundsException.h"
#include "swing/jxx.swing.RowFilter.h"
#include "swing/jxx.swing.RowSorter.h"

namespace jxx::swing
{
    template<typename M, typename I>
    class DefaultRowSorter : public ::jxx::lang::ClassBase<
        DefaultRowSorter<M, I>, RowSorter<M>>
    {
    public:
        using JxxSuper = RowSorter<M>;
        using Super = ::jxx::lang::ClassBase<DefaultRowSorter<M, I>, JxxSuper>;
        using SortKey = typename RowSorter<M>::SortKey;

        class ModelWrapper : public ::jxx::lang::ClassBase<
            ModelWrapper, ::jxx::lang::Object>
        {
        public:
            using JxxSuper = ::jxx::lang::Object;
            using Super = ::jxx::lang::ClassBase<ModelWrapper, JxxSuper>;
            ModelWrapper() : Super() {}
            ~ModelWrapper() override = default;
            virtual ::jxx::Ptr<M> getModel() const = 0;
            virtual ::jxx::lang::jint getColumnCount() const = 0;
            virtual ::jxx::lang::jint getRowCount() const = 0;
            virtual ::jxx::Ptr<::jxx::lang::Object> getValueAt(
                ::jxx::lang::jint row,
                ::jxx::lang::jint column) const = 0;
            virtual ::jxx::Ptr<I> getIdentifier(
                ::jxx::lang::jint row) const = 0;
        };

        DefaultRowSorter() : Super() {}
        ~DefaultRowSorter() override = default;

        void setModelWrapper(const ::jxx::Ptr<ModelWrapper>& wrapper)
        {
            wrapper_ = wrapper;
            rebuild();
        }

        ::jxx::Ptr<ModelWrapper> getModelWrapper() const { return wrapper_; }
        ::jxx::Ptr<M> getModel() const override
        { return wrapper_ == nullptr ? nullptr : wrapper_->getModel(); }

        void setRowFilter(const ::jxx::Ptr<RowFilter<M, I>>& filter)
        { filter_ = filter; rebuild(); }
        ::jxx::Ptr<RowFilter<M, I>> getRowFilter() const { return filter_; }

        void setSortKeys(const std::vector<::jxx::Ptr<SortKey>>& keys) override
        { sortKeys_ = keys; this->fireSortOrderChanged(); rebuild(); }
        std::vector<::jxx::Ptr<SortKey>> getSortKeys() const override
        { return sortKeys_; }

        void toggleSortOrder(::jxx::lang::jint column) override
        {
            if (column < 0 || wrapper_ == nullptr
                || column >= wrapper_->getColumnCount())
                throw ::jxx::lang::IndexOutOfBoundsException("column");
            SortOrder order = SortOrder::ASCENDING;
            if (!sortKeys_.empty()
                && sortKeys_.front()->getColumn() == column)
            {
                order = sortKeys_.front()->getSortOrder()
                    == SortOrder::ASCENDING
                    ? SortOrder::DESCENDING : SortOrder::ASCENDING;
            }
            setSortKeys({::jxx::NEW<SortKey>(column, order)});
        }

        ::jxx::lang::jint convertRowIndexToModel(
            ::jxx::lang::jint index) const override
        {
            checkView(index);
            return viewToModel_[static_cast<std::size_t>(index)];
        }

        ::jxx::lang::jint convertRowIndexToView(
            ::jxx::lang::jint index) const override
        {
            if (index < 0 || index >= getModelRowCount())
                throw ::jxx::lang::IndexOutOfBoundsException("index");
            return modelToView_[static_cast<std::size_t>(index)];
        }

        ::jxx::lang::jint getViewRowCount() const override
        { return static_cast<::jxx::lang::jint>(viewToModel_.size()); }
        ::jxx::lang::jint getModelRowCount() const override
        { return wrapper_ == nullptr ? 0 : wrapper_->getRowCount(); }

        void modelStructureChanged() override { sortKeys_.clear(); rebuild(); }
        void allRowsChanged() override { rebuild(); }
        void rowsInserted(::jxx::lang::jint, ::jxx::lang::jint) override
        { rebuild(); }
        void rowsDeleted(::jxx::lang::jint, ::jxx::lang::jint) override
        { rebuild(); }
        void rowsUpdated(::jxx::lang::jint, ::jxx::lang::jint) override
        { if (sortsOnUpdates_) rebuild(); }
        void rowsUpdated(::jxx::lang::jint firstRow,
            ::jxx::lang::jint endRow, ::jxx::lang::jint) override
        { rowsUpdated(firstRow, endRow); }

        void setSortsOnUpdates(::jxx::lang::jbool value)
        { sortsOnUpdates_ = value; }
        ::jxx::lang::jbool getSortsOnUpdates() const
        { return sortsOnUpdates_; }

    protected:
        virtual ::jxx::lang::jint compare(
            ::jxx::lang::jint row1,
            ::jxx::lang::jint row2,
            ::jxx::lang::jint column) const
        {
            const auto left = wrapper_->getValueAt(row1, column);
            const auto right = wrapper_->getValueAt(row2, column);
            if (left == nullptr || right == nullptr)
                return left == right ? 0 : (left == nullptr ? -1 : 1);
            const auto leftText = left->toString()->utf8();
            const auto rightText = right->toString()->utf8();
            return leftText < rightText ? -1 : (leftText > rightText ? 1 : 0);
        }

        void rebuild()
        {
            const auto oldCount = viewToModel_.size();
            viewToModel_.clear();
            const auto count = getModelRowCount();
            for (::jxx::lang::jint row = 0; row < count; ++row)
                viewToModel_.push_back(row);

            if (!sortKeys_.empty())
            {
                std::stable_sort(viewToModel_.begin(), viewToModel_.end(),
                    [this](::jxx::lang::jint left, ::jxx::lang::jint right)
                    {
                        for (const auto& key : sortKeys_)
                        {
                            if (key == nullptr
                                || key->getSortOrder() == SortOrder::UNSORTED)
                                continue;
                            auto result = compare(left, right, key->getColumn());
                            if (key->getSortOrder() == SortOrder::DESCENDING)
                                result = -result;
                            if (result != 0) return result < 0;
                        }
                        return left < right;
                    });
            }

            modelToView_.assign(static_cast<std::size_t>(count), -1);
            for (std::size_t view = 0; view < viewToModel_.size(); ++view)
                modelToView_[static_cast<std::size_t>(viewToModel_[view])]
                    = static_cast<::jxx::lang::jint>(view);
            this->fireRowSorterChanged(nullptr);
            (void)oldCount;
        }

    private:
        void checkView(::jxx::lang::jint index) const
        {
            if (index < 0 || index >= getViewRowCount())
                throw ::jxx::lang::IndexOutOfBoundsException("index");
        }

        ::jxx::Ptr<ModelWrapper> wrapper_;
        ::jxx::Ptr<RowFilter<M, I>> filter_;
        std::vector<::jxx::Ptr<SortKey>> sortKeys_;
        std::vector<::jxx::lang::jint> viewToModel_;
        std::vector<::jxx::lang::jint> modelToView_;
        ::jxx::lang::jbool sortsOnUpdates_ = false;
    };
}
