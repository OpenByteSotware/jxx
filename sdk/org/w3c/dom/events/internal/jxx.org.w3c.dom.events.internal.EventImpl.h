#pragma once

#include "lang/jxx.lang.ClassInfo.h"
#include "lang/jxx.lang.Object.h"
#include "org/w3c/dom/events/jxx.org.w3c.dom.events.Event.h"

namespace jxx::org::w3c::dom::events::internal {

class EventImpl final
    : public ::jxx::lang::ClassBase<
          EventImpl,
          ::jxx::lang::Object,
          ::jxx::org::w3c::dom::events::Event> {
public:
    using JxxSuper = ::jxx::lang::Object;
    using Super = ::jxx::lang::ClassBase<
        EventImpl,
        JxxSuper,
        ::jxx::org::w3c::dom::events::Event>;

    EventImpl();
    ~EventImpl() override;

    ::jxx::Ptr<::jxx::lang::String> getType() const override;
    ::jxx::Ptr<::jxx::org::w3c::dom::events::EventTarget> getTarget() const override;
    ::jxx::Ptr<::jxx::org::w3c::dom::events::EventTarget> getCurrentTarget() const override;
    ::jxx::lang::jshort getEventPhase() const override;
    ::jxx::lang::jbool getBubbles() const override;
    ::jxx::lang::jbool getCancelable() const override;
    ::jxx::lang::jlong getTimeStamp() const override;
    void stopPropagation() override;
    void preventDefault() override;
    void initEvent(
        const ::jxx::Ptr<::jxx::lang::String>& eventTypeArg,
        ::jxx::lang::jbool canBubbleArg,
        ::jxx::lang::jbool cancelableArg) override;

    ::jxx::lang::jbool isPropagationStopped() const;
    ::jxx::lang::jbool isDefaultPrevented() const;

    void setDispatchContext(
        const ::jxx::Ptr<::jxx::org::w3c::dom::events::EventTarget>& target,
        const ::jxx::Ptr<::jxx::org::w3c::dom::events::EventTarget>& currentTarget,
        ::jxx::lang::jshort phase);
    void clearDispatchContext();

private:
    ::jxx::Ptr<::jxx::lang::String> type_;
    ::jxx::Ptr<::jxx::org::w3c::dom::events::EventTarget> target_;
    ::jxx::Ptr<::jxx::org::w3c::dom::events::EventTarget> currentTarget_;
    ::jxx::lang::jshort eventPhase_ = 0;
    ::jxx::lang::jbool bubbles_ = false;
    ::jxx::lang::jbool cancelable_ = false;
    ::jxx::lang::jbool propagationStopped_ = false;
    ::jxx::lang::jbool defaultPrevented_ = false;
    ::jxx::lang::jbool dispatching_ = false;
    ::jxx::lang::jlong timeStamp_ = 0;
};

} // namespace jxx::org::w3c::dom::events::internal
