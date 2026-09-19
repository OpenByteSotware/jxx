#include "swing/event/jxx.swing.event.EventListenerList.h"

namespace jxx::swing::event
{
    ::jxx::lang::jint EventListenerList::getListenerCount() const noexcept
    {
        return static_cast<::jxx::lang::jint>(listeners_.size());
    }
}
