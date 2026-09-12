#include "org/w3c/dom/events/jxx.org.w3c.dom.events.EventException.h"

#include <utility>

#include "lang/jxx.lang.String.h"

namespace jxx::org::w3c::dom::events {

EventException::EventException(
    ::jxx::lang::jshort value,
    const ::jxx::Ptr<::jxx::lang::String>& message)
    : JxxSuper(message)
    , code(value) {
}

EventException::EventException(const EventException& other)
    : JxxSuper(other)
    , code(other.code) {
}

EventException::EventException(EventException&& other) noexcept
    : JxxSuper(std::move(other))
    , code(other.code) {
}

EventException& EventException::operator=(const EventException& other) {
    if (this != &other) {
        JxxSuper::operator=(other);
        code = other.code;
    }
    return *this;
}

EventException& EventException::operator=(EventException&& other) noexcept {
    if (this != &other) {
        JxxSuper::operator=(std::move(other));
        code = other.code;
    }
    return *this;
}

EventException::~EventException() = default;

const char* EventException::typeName() const noexcept {
    return "jxx::org::w3c::dom::events::EventException";
}

} // namespace jxx::org::w3c::dom::events
