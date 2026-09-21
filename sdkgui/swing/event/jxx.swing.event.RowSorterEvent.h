#pragma once

#include "lang/jxx.lang.buildin_array.h"
#include "util/jxx.util.EventObject.h"

namespace jxx::swing
{
    template<typename M>
    class RowSorter;
}

namespace jxx::swing::event
{
    class RowSorterEvent : public ::jxx::lang::ClassBase<
        RowSorterEvent, ::jxx::util::EventObject>
    {
    public:
        using JxxSuper = ::jxx::util::EventObject;
        using Super = ::jxx::lang::ClassBase<RowSorterEvent, JxxSuper>;

        enum class Type
        {
            SORT_ORDER_CHANGED,
            SORTED
        };

        RowSorterEvent(
            const ::jxx::Ptr<::jxx::lang::Object>& source,
            Type type,
            const ::jxx::Ptr<::jxx::lang::JxxArray<
                ::jxx::lang::jint, 1>>& previousRowIndexToModel);

        Type getType() const;
        ::jxx::lang::jint convertPreviousRowIndexToModel(
            ::jxx::lang::jint index) const;
        ::jxx::lang::jint getPreviousRowCount() const;

    private:
        Type type_;
        ::jxx::Ptr<::jxx::lang::JxxArray<::jxx::lang::jint, 1>>
            previousRowIndexToModel_;
    };
}
