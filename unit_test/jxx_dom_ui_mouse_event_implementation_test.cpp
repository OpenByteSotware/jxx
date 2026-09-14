#include <gtest/gtest.h>
#include "lang/jxx.lang.String.h"
#include "org/w3c/dom/events/jxx.org.w3c.dom.events.DocumentEvent.h"
#include "org/w3c/dom/events/jxx.org.w3c.dom.events.MouseEvent.h"
#include "org/w3c/dom/events/jxx.org.w3c.dom.events.UIEvent.h"
#include "org/w3c/dom/internal/jxx.org.w3c.dom.internal.DOMImplementationImpl.h"
#include "org/w3c/dom/jxx.org.w3c.dom.Document.h"
TEST(XmlDomUiMouseEventImplementationStage, CreatesUiEvent) {
    const auto document = ::jxx::org::w3c::dom::internal::implementation()->createDocument(nullptr, nullptr, nullptr);
    const auto creator = ::jxx::CAST<::jxx::org::w3c::dom::events::DocumentEvent>(document);
    const auto event = ::jxx::CAST<::jxx::org::w3c::dom::events::UIEvent>(creator->createEvent(::jxx::NEW<::jxx::lang::String>("UIEvents")));
    ASSERT_NE(event, nullptr);
    event->initUIEvent(::jxx::NEW<::jxx::lang::String>("activate"), true, true, nullptr, 7);
    EXPECT_EQ(event->getType()->utf8(), "activate"); EXPECT_EQ(event->getDetail(), 7); EXPECT_EQ(event->getView(), nullptr);
}
TEST(XmlDomUiMouseEventImplementationStage, CreatesMouseEvent) {
    const auto document = ::jxx::org::w3c::dom::internal::implementation()->createDocument(nullptr, nullptr, nullptr);
    const auto creator = ::jxx::CAST<::jxx::org::w3c::dom::events::DocumentEvent>(document);
    const auto event = ::jxx::CAST<::jxx::org::w3c::dom::events::MouseEvent>(creator->createEvent(::jxx::NEW<::jxx::lang::String>("MouseEvent")));
    ASSERT_NE(event, nullptr);
    event->initMouseEvent(::jxx::NEW<::jxx::lang::String>("click"), true, true, nullptr, 2, 10, 20, 30, 40, true, false, true, false, 1, nullptr);
    EXPECT_EQ(event->getDetail(), 2); EXPECT_EQ(event->getScreenX(), 10); EXPECT_EQ(event->getClientY(), 40);
    EXPECT_TRUE(event->getCtrlKey()); EXPECT_TRUE(event->getShiftKey()); EXPECT_EQ(event->getButton(), 1);
}
