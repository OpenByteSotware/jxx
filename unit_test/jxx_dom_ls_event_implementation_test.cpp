#include <gtest/gtest.h>

#include "lang/jxx.lang.String.h"
#include "org/w3c/dom/events/jxx.org.w3c.dom.events.Event.h"
#include "org/w3c/dom/internal/jxx.org.w3c.dom.internal.DOMImplementationImpl.h"
#include "org/w3c/dom/jxx.org.w3c.dom.Document.h"
#include "org/w3c/dom/ls/internal/jxx.org.w3c.dom.ls.internal.DOMImplementationLSImpl.h"
#include "org/w3c/dom/ls/internal/jxx.org.w3c.dom.ls.internal.LSLoadEventImpl.h"
#include "org/w3c/dom/ls/internal/jxx.org.w3c.dom.ls.internal.LSProgressEventImpl.h"
#include "org/w3c/dom/ls/jxx.org.w3c.dom.ls.LSInput.h"
#include "org/w3c/dom/ls/jxx.org.w3c.dom.ls.LSLoadEvent.h"
#include "org/w3c/dom/ls/jxx.org.w3c.dom.ls.LSProgressEvent.h"

TEST(XmlDomLsEventImplementationStage, LoadEventRetainsDocumentAndInput) {
    const auto document =
        ::jxx::org::w3c::dom::internal::implementation()->createDocument(
            nullptr,
            ::jxx::NEW<::jxx::lang::String>("root"),
            nullptr);

    const auto input =
        ::jxx::org::w3c::dom::ls::internal::loadSaveImplementation()
            ->createLSInput();

    const auto implementation =
        ::jxx::NEW<
            ::jxx::org::w3c::dom::ls::internal::LSLoadEventImpl>(
                document,
                input);

    const auto event =
        ::jxx::CAST<::jxx::org::w3c::dom::ls::LSLoadEvent>(
            implementation);

    ASSERT_NE(event, nullptr);
    EXPECT_EQ(event->getNewDocument(), document);
    EXPECT_EQ(event->getInput(), input);
    ASSERT_NE(event->getType(), nullptr);
    EXPECT_EQ(event->getType()->utf8(), "load");
    EXPECT_FALSE(event->getBubbles());
    EXPECT_FALSE(event->getCancelable());
}

TEST(XmlDomLsEventImplementationStage, ProgressEventRetainsProgressState) {
    const auto input =
        ::jxx::org::w3c::dom::ls::internal::loadSaveImplementation()
            ->createLSInput();

    const auto implementation =
        ::jxx::NEW<
            ::jxx::org::w3c::dom::ls::internal::LSProgressEventImpl>(
                input,
                4096,
                16384);

    const auto event =
        ::jxx::CAST<::jxx::org::w3c::dom::ls::LSProgressEvent>(
            implementation);

    ASSERT_NE(event, nullptr);
    EXPECT_EQ(event->getInput(), input);
    EXPECT_EQ(event->getPosition(), 4096);
    EXPECT_EQ(event->getTotalSize(), 16384);
    ASSERT_NE(event->getType(), nullptr);
    EXPECT_EQ(event->getType()->utf8(), "progress");
    EXPECT_FALSE(event->getBubbles());
    EXPECT_FALSE(event->getCancelable());
}
