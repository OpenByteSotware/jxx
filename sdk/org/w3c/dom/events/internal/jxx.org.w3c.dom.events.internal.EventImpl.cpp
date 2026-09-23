#include <chrono>
#include "org/w3c/dom/events/internal/jxx.org.w3c.dom.events.internal.EventImpl.h"
#include "lang/jxx.lang.String.h"
#include "org/w3c/dom/events/jxx.org.w3c.dom.events.EventTarget.h"

namespace jxx::org::w3c::dom::events::internal {

EventImpl::EventImpl()
    : timeStamp_(static_cast<::jxx::lang::jlong>(
          std::chrono::duration_cast<std::chrono::milliseconds>(
              std::chrono::system_clock::now().time_since_epoch()).count())) {
}

EventImpl::~EventImpl() = default;

::jxx::Ptr<::jxx::lang::String> EventImpl::getType() const { return type_; }
::jxx::Ptr<::jxx::org::w3c::dom::events::EventTarget> EventImpl::getTarget() const { return target_; }
::jxx::Ptr<::jxx::org::w3c::dom::events::EventTarget> EventImpl::getCurrentTarget() const { return currentTarget_; }
::jxx::lang::jshort EventImpl::getEventPhase() const { return eventPhase_; }
::jxx::lang::jbool EventImpl::getBubbles() const { return bubbles_; }
::jxx::lang::jbool EventImpl::getCancelable() const { return cancelable_; }
::jxx::lang::jlong EventImpl::getTimeStamp() const { return timeStamp_; }

void EventImpl::stopPropagation() { propagationStopped_ = true; }

void EventImpl::preventDefault() {
    if (cancelable_) defaultPrevented_ = true;
}

void EventImpl::initEvent(
    const ::jxx::Ptr<::jxx::lang::String>& eventTypeArg,
    ::jxx::lang::jbool canBubbleArg,
    ::jxx::lang::jbool cancelableArg) {
    if (dispatching_) return;
    type_ = eventTypeArg;
    bubbles_ = canBubbleArg;
    cancelable_ = cancelableArg;
    propagationStopped_ = false;
    defaultPrevented_ = false;
}

::jxx::lang::jbool EventImpl::isPropagationStopped() const { return propagationStopped_; }
::jxx::lang::jbool EventImpl::isDefaultPrevented() const { return defaultPrevented_; }

void EventImpl::setDispatchContext(
    const ::jxx::Ptr<::jxx::org::w3c::dom::events::EventTarget>& target,
    const ::jxx::Ptr<::jxx::org::w3c::dom::events::EventTarget>& currentTarget,
    ::jxx::lang::jshort phase) {
    target_ = target;
    currentTarget_ = currentTarget;
    eventPhase_ = phase;
    dispatching_ = true;
}

void EventImpl::clearDispatchContext() {
    currentTarget_.reset();
    eventPhase_ = 0;
    dispatching_ = false;
}

} // namespace jxx::org::w3c::dom::events::internal
