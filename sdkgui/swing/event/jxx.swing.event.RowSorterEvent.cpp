#include "swing/event/jxx.swing.event.RowSorterEvent.h"

#include "lang/jxx.lang.IndexOutOfBoundsException.h"
#include "lang/jxx.lang.buildin_array.h"

namespace jxx::swing::event
{
    RowSorterEvent::RowSorterEvent(
        const ::jxx::Ptr<::jxx::lang::Object>& source,
        Type type,
        const ::jxx::Ptr<::jxx::lang::JxxArray<
            ::jxx::lang::jint, 1>>& previousRowIndexToModel)
        : Super(source),
          type_(type),
          previousRowIndexToModel_(previousRowIndexToModel)
    {
    }

    RowSorterEvent::Type RowSorterEvent::getType() const
    {
        return type_;
    }

    ::jxx::lang::jint RowSorterEvent::convertPreviousRowIndexToModel(
        ::jxx::lang::jint index) const
    {
        if (previousRowIndexToModel_ == nullptr
            || index < 0
            || index >= static_cast<::jxx::lang::jint>(
                previousRowIndexToModel_->length))
            throw ::jxx::lang::IndexOutOfBoundsException("index");
        return (*previousRowIndexToModel_)[
            static_cast<std::uint32_t>(index)];
    }

    ::jxx::lang::jint RowSorterEvent::getPreviousRowCount() const
    {
        return previousRowIndexToModel_ == nullptr
            ? 0
            : static_cast<::jxx::lang::jint>(
                previousRowIndexToModel_->length);
    }
}
