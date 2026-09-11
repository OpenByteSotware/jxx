#include <gtest/gtest.h>

#include "lang/jxx.lang.String.h"
#include "org/w3c/dom/internal/jxx.org.w3c.dom.internal.DOMImplementationImpl.h"
#include "org/w3c/dom/jxx.org.w3c.dom.Document.h"
#include "org/w3c/dom/jxx.org.w3c.dom.Element.h"
#include "org/w3c/dom/jxx.org.w3c.dom.Node.h"

TEST(XmlDomNodeMoveSemanticsFix, AppendChildMovesExistingNode) {
    const auto document =
        ::jxx::org::w3c::dom::internal::implementation()->createDocument(
            nullptr,
            ::jxx::NEW<::jxx::lang::String>("root"),
            nullptr);
    const auto root = document->getDocumentElement();
    const auto child = document->createElement(
        ::jxx::NEW<::jxx::lang::String>("child"));
    const auto childNode =
        ::jxx::CAST<::jxx::org::w3c::dom::Node>(child);

    root->appendChild(childNode);

    EXPECT_TRUE(childNode->getParentNode()->isSameNode(
        ::jxx::CAST<::jxx::org::w3c::dom::Node>(root)));
    EXPECT_TRUE(root->getFirstChild()->isSameNode(childNode));
}

TEST(XmlDomNodeMoveSemanticsFix, ReplaceChildUpdatesParentTree) {
    const auto document =
        ::jxx::org::w3c::dom::internal::implementation()->createDocument(
            nullptr,
            ::jxx::NEW<::jxx::lang::String>("root"),
            nullptr);
    const auto root = document->getDocumentElement();
    const auto oldNode = ::jxx::CAST<::jxx::org::w3c::dom::Node>(
        document->createElement(::jxx::NEW<::jxx::lang::String>("old")));
    root->appendChild(oldNode);
    const auto newNode = ::jxx::CAST<::jxx::org::w3c::dom::Node>(
        document->createElement(::jxx::NEW<::jxx::lang::String>("new")));

    root->replaceChild(newNode, oldNode);

    ASSERT_NE(root->getFirstChild(), nullptr);
    EXPECT_EQ(root->getFirstChild()->getNodeName()->utf8(), "new");
}
