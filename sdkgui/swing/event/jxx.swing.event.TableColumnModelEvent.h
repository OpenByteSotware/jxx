#pragma once

#include "util/jxx.util.EventObject.h"

namespace jxx::swing::table
{
    class TableColumnModel;
}

namespace jxx::swing::event
{
    class TableColumnModelEvent : public ::jxx::lang::ClassBase<
        TableColumnModelEvent, ::jxx::util::EventObject>
    {
    public:
        using JxxSuper = ::jxx::util::EventObject;
        using Super = ::jxx::lang::ClassBase<TableColumnModelEvent, JxxSuper>;

        TableColumnModelEvent(
            const ::jxx::Ptr<::jxx::swing::table::TableColumnModel>& source,
            ::jxx::lang::jint fromIndex,
            ::jxx::lang::jint toIndex);

        ::jxx::lang::jint getFromIndex() const;
        ::jxx::lang::jint getToIndex() const;

    private:
        ::jxx::lang::jint fromIndex_;
        ::jxx::lang::jint toIndex_;
    };
}
