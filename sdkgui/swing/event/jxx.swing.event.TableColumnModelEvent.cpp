#include "swing/event/jxx.swing.event.TableColumnModelEvent.h"

#include "swing/table/jxx.swing.table.TableColumnModel.h"

namespace jxx::swing::event
{
    TableColumnModelEvent::TableColumnModelEvent(
        const ::jxx::Ptr<::jxx::swing::table::TableColumnModel>& source,
        ::jxx::lang::jint fromIndex,
        ::jxx::lang::jint toIndex)
        : Super(::jxx::CAST<::jxx::lang::Object>(source)),
          fromIndex_(fromIndex),
          toIndex_(toIndex)
    {
    }

    ::jxx::lang::jint TableColumnModelEvent::getFromIndex() const
    {
        return fromIndex_;
    }

    ::jxx::lang::jint TableColumnModelEvent::getToIndex() const
    {
        return toIndex_;
    }
}
