#include <gtest/gtest.h>

#include "lang/jxx.lang.String.h"
#include "org/w3c/dom/events/jxx.org.w3c.dom.events.DocumentEvent.h"
#include "org/w3c/dom/events/jxx.org.w3c.dom.events.Event.h"
#include "org/w3c/dom/internal/jxx.org.w3c.dom.internal.DOMImplementationImpl.h"
#include "org/w3c/dom/jxx.org.w3c.dom.DOMException.h"
#include "org/w3c/dom/jxx.org.w3c.dom.Document.h"

TEST(XmlDomDocumentEventCreationStage, DocumentImplementsDocumentEvent) {
    const auto document = ::jxx::org::w3c::dom::internal::implementation()
        ->createDocument(nullptr, nullptr, nullptr);
    ASSERT_NE(document, nullptr);
    const auto documentEvent =
        ::jxx::CAST<::jxx::org::w3c::dom::events::DocumentEvent>(document);
    EXPECT_NE(documentEvent, nullptr);
}

TEST(XmlDomDocumentEventCreationStage, CreatesIndependentBaseEvents) {
    const auto document = ::jxx::org::w3c::dom::internal::implementation()
        ->createDocument(nullptr, nullptr, nullptr);
    const auto documentEvent =
        ::jxx::CAST<::jxx::org::w3c::dom::events::DocumentEvent>(document);
    ASSERT_NE(documentEvent, nullptr);
    const auto first = documentEvent->createEvent(
        ::jxx::NEW<::jxx::lang::String>("Event"));
    const auto second = documentEvent->createEvent(
        ::jxx::NEW<::jxx::lang::String>("EVENTS"));
    ASSERT_NE(first, nullptr);
    ASSERT_NE(second, nullptr);
    EXPECT_NE(first, second);
    first->initEvent(::jxx::NEW<::jxx::lang::String>("sample"), true, true);
    EXPECT_EQ(first->getType()->utf8(), "sample");
    EXPECT_EQ(second->getType(), nullptr);
}

TEST(XmlDomDocumentEventCreationStage, RejectsUnsupportedEventInterface) {
    const auto document = ::jxx::org::w3c::dom::internal::implementation()
        ->createDocument(nullptr, nullptr, nullptr);
    const auto documentEvent =
        ::jxx::CAST<::jxx::org::w3c::dom::events::DocumentEvent>(document);
    ASSERT_NE(documentEvent, nullptr);
    try {
        documentEvent->createEvent(
            ::jxx::NEW<::jxx::lang::String>("UnsupportedEvents"));
        FAIL() << "Expected DOMException";
    } catch (const ::jxx::org::w3c::dom::DOMException& exception) {
        EXPECT_EQ(exception.code,
            ::jxx::org::w3c::dom::DOMException::NOT_SUPPORTED_ERR);
    }
}
