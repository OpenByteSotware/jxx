#include "swing/event/jxx.swing.event.ListSelectionEvent.h"
namespace jxx::swing::event
{
    ListSelectionEvent::ListSelectionEvent(
        const ::jxx::Ptr<::jxx::lang::Object>& source,
        ::jxx::lang::jint firstIndex, ::jxx::lang::jint lastIndex,
        ::jxx::lang::jbool isAdjusting)
        : Super(source), firstIndex_(firstIndex), lastIndex_(lastIndex),
          isAdjusting_(isAdjusting) {}
    ::jxx::lang::jint ListSelectionEvent::getFirstIndex() const { return firstIndex_; }
    ::jxx::lang::jint ListSelectionEvent::getLastIndex() const { return lastIndex_; }
    ::jxx::lang::jbool ListSelectionEvent::getValueIsAdjusting() const { return isAdjusting_; }
}
