#pragma once

#include <algorithm>
#include <vector>

#include "lang/jxx.lang.ClassInfo.h"
#include "lang/jxx.lang.Object.h"
#include "lang/jxx.lang.buildin_array.h"
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

        RowSorter() : Super() {}
        ~RowSorter() override = default;

        virtual ::jxx::Ptr<M> getModel() const = 0;
        virtual void toggleSortOrder(::jxx::lang::jint column) = 0;
        virtual ::jxx::lang::jint convertRowIndexToModel(
            ::jxx::lang::jint index) const = 0;
        virtual ::jxx::lang::jint convertRowIndexToView(
            ::jxx::lang::jint index) const = 0;
        virtual ::jxx::lang::jint getViewRowCount() const = 0;
        virtual ::jxx::lang::jint getModelRowCount() const = 0;
        virtual void modelStructureChanged() = 0;
        virtual void allRowsChanged() = 0;
        virtual void rowsInserted(
            ::jxx::lang::jint firstRow,
            ::jxx::lang::jint endRow) = 0;
        virtual void rowsDeleted(
            ::jxx::lang::jint firstRow,
            ::jxx::lang::jint endRow) = 0;
        virtual void rowsUpdated(
            ::jxx::lang::jint firstRow,
            ::jxx::lang::jint endRow) = 0;
        virtual void rowsUpdated(
            ::jxx::lang::jint firstRow,
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
            listeners_.erase(
                std::remove(listeners_.begin(), listeners_.end(), listener),
                listeners_.end());
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
        void fireEvent(
            ::jxx::swing::event::RowSorterEvent::Type type,
            const IntArray& previousMapping)
        {
            auto event = ::jxx::NEW<
                ::jxx::swing::event::RowSorterEvent>(
                    ::jxx::CAST<::jxx::lang::Object>(this->thisPtr()),
                    type,
                    previousMapping);
            const auto listeners = listeners_;
            for (const auto& listener : listeners)
                if (listener != nullptr) listener->sorterChanged(event);
        }

        std::vector<::jxx::Ptr<
            ::jxx::swing::event::RowSorterListener>> listeners_;
    };
}
