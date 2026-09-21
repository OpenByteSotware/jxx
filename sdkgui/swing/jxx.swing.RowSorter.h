#pragma once

#include <algorithm>
#include <vector>

#include "lang/jxx.lang.ClassInfo.h"
#include "lang/jxx.lang.IllegalArgumentException.h"
#include "lang/jxx.lang.Object.h"
#include "lang/jxx.lang.buildin_array.h"
#include "swing/jxx.swing.SortOrder.h"
#include "swing/event/jxx.swing.event.RowSorterEvent.h"
#include "swing/event/jxx.swing.event.RowSorterListener.h"

namespace jxx::swing
{
    template<typename M>
    class RowSorter : public ::jxx::lang::ClassBase<
        RowSorter<M>, ::jxx::lang::Object>
    {
    public:
        using JxxSuper = ::jxx::lang::Object;
        using Super = ::jxx::lang::ClassBase<RowSorter<M>, JxxSuper>;
        using IntArray = ::jxx::Ptr<::jxx::lang::JxxArray<
            ::jxx::lang::jint, 1>>;

        class SortKey : public ::jxx::lang::ClassBase<
            SortKey, ::jxx::lang::Object>
        {
        public:
            using JxxSuper = ::jxx::lang::Object;
            using Super = ::jxx::lang::ClassBase<SortKey, JxxSuper>;

            SortKey(::jxx::lang::jint column, SortOrder sortOrder)
                : Super(), column_(column), sortOrder_(sortOrder)
            {
                if (column < 0)
                    throw ::jxx::lang::IllegalArgumentException("column");
            }

            ::jxx::lang::jint getColumn() const { return column_; }
            SortOrder getSortOrder() const { return sortOrder_; }

            ::jxx::lang::jint hashCode() const override
            {
                return column_ * 31
                    + static_cast<::jxx::lang::jint>(sortOrder_);
            }

            ::jxx::lang::jbool equals(
                const ::jxx::Ptr<::jxx::lang::Object>& object) const override
            {
                const auto other = ::jxx::CAST<SortKey>(object);
                return other != nullptr
                    && other->column_ == column_
                    && other->sortOrder_ == sortOrder_;
            }

        private:
            ::jxx::lang::jint column_;
            SortOrder sortOrder_;
        };

        RowSorter() : Super() {}
        ~RowSorter() override = default;

        virtual ::jxx::Ptr<M> getModel() const = 0;
        virtual void toggleSortOrder(::jxx::lang::jint column) = 0;
        virtual std::vector<::jxx::Ptr<SortKey>> getSortKeys() const = 0;
        virtual void setSortKeys(
            const std::vector<::jxx::Ptr<SortKey>>& keys) = 0;
        virtual ::jxx::lang::jint convertRowIndexToModel(
            ::jxx::lang::jint index) const = 0;
        virtual ::jxx::lang::jint convertRowIndexToView(
            ::jxx::lang::jint index) const = 0;
        virtual ::jxx::lang::jint getViewRowCount() const = 0;
        virtual ::jxx::lang::jint getModelRowCount() const = 0;
        virtual void modelStructureChanged() = 0;
        virtual void allRowsChanged() = 0;
        virtual void rowsInserted(::jxx::lang::jint firstRow,
            ::jxx::lang::jint endRow) = 0;
        virtual void rowsDeleted(::jxx::lang::jint firstRow,
            ::jxx::lang::jint endRow) = 0;
        virtual void rowsUpdated(::jxx::lang::jint firstRow,
            ::jxx::lang::jint endRow) = 0;
        virtual void rowsUpdated(::jxx::lang::jint firstRow,
            ::jxx::lang::jint endRow,
            ::jxx::lang::jint column) = 0;

        void addRowSorterListener(
            const ::jxx::Ptr<::jxx::swing::event::RowSorterListener>& listener)
        {
            if (listener != nullptr) listeners_.push_back(listener);
        }

        void removeRowSorterListener(
            const ::jxx::Ptr<::jxx::swing::event::RowSorterListener>& listener)
        {
            listeners_.erase(std::remove(listeners_.begin(),
                listeners_.end(), listener), listeners_.end());
        }

    protected:
        void fireSortOrderChanged()
        {
            fireEvent(::jxx::swing::event::RowSorterEvent::Type::
                SORT_ORDER_CHANGED, nullptr);
        }

        void fireRowSorterChanged(const IntArray& previousMapping)
        {
            fireEvent(::jxx::swing::event::RowSorterEvent::Type::SORTED,
                previousMapping);
        }

    private:
        void fireEvent(::jxx::swing::event::RowSorterEvent::Type type,
            const IntArray& previousMapping)
        {
            auto event = ::jxx::NEW<::jxx::swing::event::RowSorterEvent>(
                ::jxx::CAST<::jxx::lang::Object>(this->thisPtr()),
                type, previousMapping);
            const auto listeners = listeners_;
            for (const auto& listener : listeners)
                if (listener != nullptr) listener->sorterChanged(event);
        }

        std::vector<::jxx::Ptr<::jxx::swing::event::RowSorterListener>>
            listeners_;
    };
}
