#include <gtest/gtest.h>

#include "org/w3c/dom/internal/jxx.org.w3c.dom.internal.DOMImplementationImpl.h"
#include "org/w3c/dom/jxx.org.w3c.dom.Document.h"
#include "org/w3c/dom/views/jxx.org.w3c.dom.views.AbstractView.h"
#include "org/w3c/dom/views/jxx.org.w3c.dom.views.DocumentView.h"

TEST(XmlDomViewsStage, DocumentProvidesDefaultView) {
    const auto document =
        ::jxx::org::w3c::dom::internal::implementation()->createDocument(
            nullptr,
            nullptr,
            nullptr);
    const auto documentView =
        ::jxx::CAST<::jxx::org::w3c::dom::views::DocumentView>(document);
    ASSERT_NE(documentView, nullptr);
    const auto view = documentView->getDefaultView();
    ASSERT_NE(view, nullptr);
    EXPECT_EQ(view->getDocument(), documentView);
}

TEST(XmlDomViewsStage, EachDefaultViewReferencesSameDocument) {
    const auto document =
        ::jxx::org::w3c::dom::internal::implementation()->createDocument(
            nullptr,
            nullptr,
            nullptr);
    const auto documentView =
        ::jxx::CAST<::jxx::org::w3c::dom::views::DocumentView>(document);
    ASSERT_NE(documentView, nullptr);
    const auto first = documentView->getDefaultView();
    const auto second = documentView->getDefaultView();
    ASSERT_NE(first, nullptr);
    ASSERT_NE(second, nullptr);
    EXPECT_EQ(first->getDocument(), documentView);
    EXPECT_EQ(second->getDocument(), documentView);
}
