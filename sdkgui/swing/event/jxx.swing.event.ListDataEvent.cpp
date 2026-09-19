#include "swing/event/jxx.swing.event.ListDataEvent.h"
#include <algorithm>
namespace jxx::swing::event {
ListDataEvent::ListDataEvent(const ::jxx::Ptr<::jxx::lang::Object>& source, ::jxx::lang::jint type,
    ::jxx::lang::jint index0, ::jxx::lang::jint index1)
    : Super(source), type_(type), index0_(std::min(index0, index1)), index1_(std::max(index0, index1)) {}
::jxx::lang::jint ListDataEvent::getType() const { return type_; }
::jxx::lang::jint ListDataEvent::getIndex0() const { return index0_; }
::jxx::lang::jint ListDataEvent::getIndex1() const { return index1_; }
}
