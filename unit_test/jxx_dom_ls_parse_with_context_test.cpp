#include <gtest/gtest.h>

#include "lang/jxx.lang.String.h"
#include "org/w3c/dom/internal/jxx.org.w3c.dom.internal.DOMImplementationImpl.h"
#include "org/w3c/dom/jxx.org.w3c.dom.Document.h"
#include "org/w3c/dom/jxx.org.w3c.dom.Element.h"
#include "org/w3c/dom/jxx.org.w3c.dom.Node.h"
#include "org/w3c/dom/ls/internal/jxx.org.w3c.dom.ls.internal.DOMImplementationLSImpl.h"
#include "org/w3c/dom/ls/jxx.org.w3c.dom.ls.DOMImplementationLS.h"
#include "org/w3c/dom/ls/jxx.org.w3c.dom.ls.LSInput.h"
#include "org/w3c/dom/ls/jxx.org.w3c.dom.ls.LSParser.h"

namespace {

::jxx::Ptr<::jxx::org::w3c::dom::ls::LSParser> parser() {
    return ::jxx::org::w3c::dom::ls::internal::loadSaveImplementation()
        ->createLSParser(
            ::jxx::org::w3c::dom::ls::DOMImplementationLS::MODE_SYNCHRONOUS,
            nullptr);
}

::jxx::Ptr<::jxx::org::w3c::dom::ls::LSInput> fragment(const char* value) {
    const auto input =
        ::jxx::org::w3c::dom::ls::internal::loadSaveImplementation()
            ->createLSInput();
    input->setStringData(::jxx::NEW<::jxx::lang::String>(value));
    return input;
}

::jxx::Ptr<::jxx::org::w3c::dom::Document> document() {
    return ::jxx::org::w3c::dom::internal::implementation()->createDocument(
        nullptr,
        ::jxx::NEW<::jxx::lang::String>("root"),
        nullptr);
}

} // namespace

TEST(XmlDomLsParseWithContextStage, AppendsFragmentChildren) {
    const auto doc = document();
    const auto root = doc->getDocumentElement();
    const auto result = parser()->parseWithContext(
        fragment("<first/><second/>"),
        ::jxx::CAST<::jxx::org::w3c::dom::Node>(root),
        ::jxx::org::w3c::dom::ls::LSParser::ACTION_APPEND_AS_CHILDREN);

    ASSERT_NE(result, nullptr);
    EXPECT_EQ(result->getNodeName()->utf8(), "first");
    EXPECT_EQ(root->getFirstChild()->getNodeName()->utf8(), "first");
    EXPECT_EQ(root->getLastChild()->getNodeName()->utf8(), "second");
}

TEST(XmlDomLsParseWithContextStage, ReplacesExistingChildren) {
    const auto doc = document();
    const auto root = doc->getDocumentElement();
    root->appendChild(::jxx::CAST<::jxx::org::w3c::dom::Node>(
        doc->createElement(::jxx::NEW<::jxx::lang::String>("old"))));

    parser()->parseWithContext(
        fragment("<replacement/>"),
        ::jxx::CAST<::jxx::org::w3c::dom::Node>(root),
        ::jxx::org::w3c::dom::ls::LSParser::ACTION_REPLACE_CHILDREN);

    const auto firstChild = root->getFirstChild();
    const auto lastChild = root->getLastChild();
    ASSERT_NE(firstChild, nullptr);
    ASSERT_NE(lastChild, nullptr);
    EXPECT_EQ(firstChild->getNodeName()->utf8(), "replacement");
    EXPECT_TRUE(firstChild->isSameNode(lastChild));
    EXPECT_EQ(firstChild->getNextSibling(), nullptr);
}

TEST(XmlDomLsParseWithContextStage, ReplacesContextNode) {
    const auto doc = document();
    const auto root = doc->getDocumentElement();
    const auto old = doc->createElement(
        ::jxx::NEW<::jxx::lang::String>("old"));
    root->appendChild(::jxx::CAST<::jxx::org::w3c::dom::Node>(old));

    const auto result = parser()->parseWithContext(
        fragment("<new/>"),
        ::jxx::CAST<::jxx::org::w3c::dom::Node>(old),
        ::jxx::org::w3c::dom::ls::LSParser::ACTION_REPLACE);

    ASSERT_NE(result, nullptr);
    EXPECT_EQ(result->getNodeName()->utf8(), "new");
    EXPECT_EQ(root->getFirstChild()->getNodeName()->utf8(), "new");
}
