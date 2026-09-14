#include <gtest/gtest.h>

#include "lang/jxx.lang.ClassInfo.h"
#include "lang/jxx.lang.Object.h"
#include "lang/jxx.lang.String.h"
#include "org/w3c/dom/events/jxx.org.w3c.dom.events.DocumentEvent.h"
#include "org/w3c/dom/events/jxx.org.w3c.dom.events.Event.h"
#include "org/w3c/dom/events/jxx.org.w3c.dom.events.EventException.h"
#include "org/w3c/dom/events/jxx.org.w3c.dom.events.EventListener.h"
#include "org/w3c/dom/events/jxx.org.w3c.dom.events.EventTarget.h"
#include "org/w3c/dom/internal/jxx.org.w3c.dom.internal.DOMImplementationImpl.h"
#include "org/w3c/dom/jxx.org.w3c.dom.Document.h"
#include "org/w3c/dom/jxx.org.w3c.dom.Element.h"

namespace {

class CountingListener final
    : public ::jxx::lang::ClassBase<
          CountingListener,
          ::jxx::lang::Object,
          ::jxx::org::w3c::dom::events::EventListener> {
public:
    void handleEvent(
        const ::jxx::Ptr<::jxx::org::w3c::dom::events::Event>& event) override {
        ++count;
        phase = event->getEventPhase();
        currentTarget = event->getCurrentTarget();
    }

    ::jxx::lang::jint count = 0;
    ::jxx::lang::jshort phase = 0;
    ::jxx::Ptr<::jxx::org::w3c::dom::events::EventTarget> currentTarget;
};

} // namespace

TEST(XmlDomEventTargetStage, RegistersSuppressesDuplicatesAndRemoves) {
    const auto document = ::jxx::org::w3c::dom::internal::implementation()
        ->createDocument(nullptr, ::jxx::NEW<::jxx::lang::String>("root"), nullptr);
    const auto target = ::jxx::CAST<::jxx::org::w3c::dom::events::EventTarget>(
        document->getDocumentElement());
    const auto creator = ::jxx::CAST<::jxx::org::w3c::dom::events::DocumentEvent>(document);
    const auto listener = ::jxx::NEW<CountingListener>();
    const auto type = ::jxx::NEW<::jxx::lang::String>("Sample");
    target->addEventListener(type, listener, false);
    target->addEventListener(type, listener, false);
    const auto first = creator->createEvent(::jxx::NEW<::jxx::lang::String>("Event"));
    first->initEvent(::jxx::NEW<::jxx::lang::String>("sample"), false, false);
    EXPECT_TRUE(target->dispatchEvent(first));
    EXPECT_EQ(listener->count, 1);
    EXPECT_EQ(listener->phase, ::jxx::org::w3c::dom::events::Event::AT_TARGET);
    EXPECT_NE(listener->currentTarget, nullptr);
    target->removeEventListener(type, listener, false);
    const auto second = creator->createEvent(::jxx::NEW<::jxx::lang::String>("Event"));
    second->initEvent(::jxx::NEW<::jxx::lang::String>("sample"), false, false);
    EXPECT_TRUE(target->dispatchEvent(second));
    EXPECT_EQ(listener->count, 1);
}

TEST(XmlDomEventTargetStage, RejectsUninitializedEvent) {
    const auto document = ::jxx::org::w3c::dom::internal::implementation()
        ->createDocument(nullptr, ::jxx::NEW<::jxx::lang::String>("root"), nullptr);
    const auto target = ::jxx::CAST<::jxx::org::w3c::dom::events::EventTarget>(
        document->getDocumentElement());
    const auto creator = ::jxx::CAST<::jxx::org::w3c::dom::events::DocumentEvent>(document);
    try {
        target->dispatchEvent(creator->createEvent(
            ::jxx::NEW<::jxx::lang::String>("Event")));
        FAIL() << "Expected EventException";
    } catch (const ::jxx::org::w3c::dom::events::EventException& exception) {
        EXPECT_EQ(exception.code,
            ::jxx::org::w3c::dom::events::EventException::UNSPECIFIED_EVENT_TYPE_ERR);
    }
}
