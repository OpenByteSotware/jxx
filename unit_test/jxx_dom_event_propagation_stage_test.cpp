#include <gtest/gtest.h>

#include <string>
#include <vector>

#include "lang/jxx.lang.ClassInfo.h"
#include "lang/jxx.lang.Object.h"
#include "lang/jxx.lang.String.h"
#include "org/w3c/dom/events/jxx.org.w3c.dom.events.DocumentEvent.h"
#include "org/w3c/dom/events/jxx.org.w3c.dom.events.Event.h"
#include "org/w3c/dom/events/jxx.org.w3c.dom.events.EventListener.h"
#include "org/w3c/dom/events/jxx.org.w3c.dom.events.EventTarget.h"
#include "org/w3c/dom/internal/jxx.org.w3c.dom.internal.DOMImplementationImpl.h"
#include "org/w3c/dom/jxx.org.w3c.dom.Document.h"
#include "org/w3c/dom/jxx.org.w3c.dom.Element.h"
#include "org/w3c/dom/jxx.org.w3c.dom.Node.h"

namespace {

class RecordingListener final
    : public ::jxx::lang::ClassBase<
          RecordingListener,
          ::jxx::lang::Object,
          ::jxx::org::w3c::dom::events::EventListener> {
public:
    RecordingListener(
        std::vector<std::string>* calls,
        std::string label,
        bool stop = false)
        : calls_(calls)
        , label_(std::move(label))
        , stop_(stop) {
    }

    void handleEvent(
        const ::jxx::Ptr<::jxx::org::w3c::dom::events::Event>& event) override {
        calls_->push_back(label_ + ":" +
            std::to_string(event->getEventPhase()));
        if (stop_) {
            event->stopPropagation();
        }
    }

private:
    std::vector<std::string>* calls_;
    std::string label_;
    bool stop_;
};

} // namespace

TEST(XmlDomEventPropagationStage, CapturesTargetsAndBubblesInOrder) {
    const auto document = ::jxx::org::w3c::dom::internal::implementation()
        ->createDocument(nullptr, ::jxx::NEW<::jxx::lang::String>("root"), nullptr);
    const auto root = document->getDocumentElement();
    const auto child = document->createElement(
        ::jxx::NEW<::jxx::lang::String>("child"));
    root->appendChild(::jxx::CAST<::jxx::org::w3c::dom::Node>(child));

    const auto documentTarget = ::jxx::CAST<::jxx::org::w3c::dom::events::EventTarget>(document);
    const auto rootTarget = ::jxx::CAST<::jxx::org::w3c::dom::events::EventTarget>(root);
    const auto childTarget = ::jxx::CAST<::jxx::org::w3c::dom::events::EventTarget>(child);
    const auto type = ::jxx::NEW<::jxx::lang::String>("sample");
    std::vector<std::string> calls;

    documentTarget->addEventListener(type, ::jxx::NEW<RecordingListener>(&calls, "document-capture"), true);
    rootTarget->addEventListener(type, ::jxx::NEW<RecordingListener>(&calls, "root-capture"), true);
    childTarget->addEventListener(type, ::jxx::NEW<RecordingListener>(&calls, "target-capture"), true);
    childTarget->addEventListener(type, ::jxx::NEW<RecordingListener>(&calls, "target-bubble"), false);
    rootTarget->addEventListener(type, ::jxx::NEW<RecordingListener>(&calls, "root-bubble"), false);
    documentTarget->addEventListener(type, ::jxx::NEW<RecordingListener>(&calls, "document-bubble"), false);

    const auto creator = ::jxx::CAST<::jxx::org::w3c::dom::events::DocumentEvent>(document);
    const auto event = creator->createEvent(::jxx::NEW<::jxx::lang::String>("Event"));
    event->initEvent(type, true, false);
    EXPECT_TRUE(childTarget->dispatchEvent(event));

    EXPECT_EQ(calls, (std::vector<std::string>{
        "document-capture:1", "root-capture:1",
        "target-capture:2", "target-bubble:2",
        "root-bubble:3", "document-bubble:3"}));
    EXPECT_EQ(event->getCurrentTarget(), nullptr);
    EXPECT_EQ(event->getEventPhase(), 0);
}

TEST(XmlDomEventPropagationStage, StopPropagationPreventsLaterNodes) {
    const auto document = ::jxx::org::w3c::dom::internal::implementation()
        ->createDocument(nullptr, ::jxx::NEW<::jxx::lang::String>("root"), nullptr);
    const auto root = document->getDocumentElement();
    const auto child = document->createElement(
        ::jxx::NEW<::jxx::lang::String>("child"));
    root->appendChild(::jxx::CAST<::jxx::org::w3c::dom::Node>(child));
    const auto rootTarget = ::jxx::CAST<::jxx::org::w3c::dom::events::EventTarget>(root);
    const auto childTarget = ::jxx::CAST<::jxx::org::w3c::dom::events::EventTarget>(child);
    const auto type = ::jxx::NEW<::jxx::lang::String>("sample");
    std::vector<std::string> calls;
    rootTarget->addEventListener(type, ::jxx::NEW<RecordingListener>(&calls, "stop", true), true);
    childTarget->addEventListener(type, ::jxx::NEW<RecordingListener>(&calls, "target"), false);
    const auto creator = ::jxx::CAST<::jxx::org::w3c::dom::events::DocumentEvent>(document);
    const auto event = creator->createEvent(::jxx::NEW<::jxx::lang::String>("Event"));
    event->initEvent(type, true, false);
    EXPECT_TRUE(childTarget->dispatchEvent(event));
    EXPECT_EQ(calls, (std::vector<std::string>{"stop:1"}));
}
