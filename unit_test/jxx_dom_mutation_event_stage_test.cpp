#include <gtest/gtest.h>

#include "lang/jxx.lang.String.h"
#include "org/w3c/dom/events/jxx.org.w3c.dom.events.DocumentEvent.h"
#include "org/w3c/dom/events/jxx.org.w3c.dom.events.Event.h"
#include "org/w3c/dom/events/jxx.org.w3c.dom.events.MutationEvent.h"
#include "org/w3c/dom/internal/jxx.org.w3c.dom.internal.DOMImplementationImpl.h"
#include "org/w3c/dom/jxx.org.w3c.dom.Document.h"
#include "org/w3c/dom/jxx.org.w3c.dom.Element.h"
#include "org/w3c/dom/jxx.org.w3c.dom.Node.h"

TEST(XmlDomMutationEventStage, CreatesAndInitializesMutationEvent) {
    const auto document =
        ::jxx::org::w3c::dom::internal::implementation()->createDocument(
            nullptr,
            ::jxx::NEW<::jxx::lang::String>("root"),
            nullptr);
    ASSERT_NE(document, nullptr);

    const auto documentEvent =
        ::jxx::CAST<::jxx::org::w3c::dom::events::DocumentEvent>(document);
    ASSERT_NE(documentEvent, nullptr);

    const auto event = documentEvent->createEvent(
        ::jxx::NEW<::jxx::lang::String>("MutationEvents"));
    const auto mutation =
        ::jxx::CAST<::jxx::org::w3c::dom::events::MutationEvent>(event);
    ASSERT_NE(mutation, nullptr);

    const auto related = ::jxx::CAST<::jxx::org::w3c::dom::Node>(
        document->getDocumentElement());
    const auto previous = ::jxx::NEW<::jxx::lang::String>("old");
    const auto next = ::jxx::NEW<::jxx::lang::String>("new");
    const auto attribute = ::jxx::NEW<::jxx::lang::String>("id");

    mutation->initMutationEvent(
        ::jxx::NEW<::jxx::lang::String>("DOMAttrModified"),
        true,
        true,
        related,
        previous,
        next,
        attribute,
        ::jxx::org::w3c::dom::events::MutationEvent::MODIFICATION);

    EXPECT_EQ(mutation->getType()->utf8(), "DOMAttrModified");
    EXPECT_TRUE(mutation->getBubbles());
    EXPECT_TRUE(mutation->getCancelable());
    EXPECT_TRUE(mutation->getRelatedNode()->isSameNode(related));
    EXPECT_EQ(mutation->getPrevValue()->utf8(), "old");
    EXPECT_EQ(mutation->getNewValue()->utf8(), "new");
    EXPECT_EQ(mutation->getAttrName()->utf8(), "id");
    EXPECT_EQ(
        mutation->getAttrChange(),
        ::jxx::org::w3c::dom::events::MutationEvent::MODIFICATION);
}

TEST(XmlDomMutationEventStage, ExposesChangeConstants) {
    EXPECT_EQ(::jxx::org::w3c::dom::events::MutationEvent::MODIFICATION, 1);
    EXPECT_EQ(::jxx::org::w3c::dom::events::MutationEvent::ADDITION, 2);
    EXPECT_EQ(::jxx::org::w3c::dom::events::MutationEvent::REMOVAL, 3);
}
