#include <sstream>
#include <string>

#include "lang/jxx.lang.NullPointerException.h"
#include "lang/jxx.lang.String.h"
#include "util/jxx.util.EventObject.h"

namespace jxx::util {

EventObject::EventObject(
    const ::jxx::Ptr<::jxx::lang::Object>& source)
    : source_(source) {
    if (source_ == nullptr) {
        throw ::jxx::lang::NullPointerException();
    }
}

EventObject::~EventObject() = default;

::jxx::Ptr<::jxx::lang::Object>
EventObject::getSource() const {
    return source_;
}

void EventObject::setSource(
    const ::jxx::Ptr<::jxx::lang::Object>& source) {
    if (source == nullptr) throw ::jxx::lang::NullPointerException();
    source_ = source;
}

::jxx::Ptr<::jxx::lang::String>
EventObject::toString() const {
    std::ostringstream output;
    output << "EventObject[source=";
    const auto value = source_->toString();
    output << (value == nullptr ? "null" : value->utf8());
    output << ']';
    return ::jxx::NEW<::jxx::lang::String>(output.str());
}

::jxx::Ptr<::jxx::lang::Object>
EventObject::cloneImpl() const {
    return ::jxx::CAST<::jxx::lang::Object>(
        ::jxx::NEW<EventObject>(source_));
}


void EventObject::writeObject(
    const ::jxx::Ptr<::jxx::io::ObjectOutputStream>& output) {
    (void)output;
}

void EventObject::readObject(
    const ::jxx::Ptr<::jxx::io::ObjectInputStream>& input) {
    (void)input;
}

void EventObject::readObjectNoData() {
}

} // namespace jxx::util
