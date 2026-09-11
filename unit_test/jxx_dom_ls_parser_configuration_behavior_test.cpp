#include <gtest/gtest.h>

#include "lang/jxx.lang.Boolean.h"
#include "lang/jxx.lang.Object.h"
#include "lang/jxx.lang.String.h"
#include "org/w3c/dom/jxx.org.w3c.dom.DOMConfiguration.h"
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

::jxx::Ptr<::jxx::org::w3c::dom::ls::LSInput> input(const char* text) {
    const auto value =
        ::jxx::org::w3c::dom::ls::internal::loadSaveImplementation()
            ->createLSInput();
    value->setStringData(::jxx::NEW<::jxx::lang::String>(text));
    return value;
}

void setFlag(
    const ::jxx::Ptr<::jxx::org::w3c::dom::DOMConfiguration>& configuration,
    const char* name,
    ::jxx::lang::jbool value) {
    configuration->setParameter(
        ::jxx::NEW<::jxx::lang::String>(name),
        ::jxx::CAST<::jxx::lang::Object>(
            ::jxx::lang::Boolean::valueOf(value)));
}

} // namespace

TEST(XmlDomLsParserConfigurationBehaviorStage, CommentsArePreservedByDefault) {
    const auto document = parser()->parse(input("<root><!--visible--><child/></root>"));
    const auto root = document->getDocumentElement();
    ASSERT_NE(root, nullptr);
    ASSERT_NE(root->getFirstChild(), nullptr);
    EXPECT_EQ(root->getFirstChild()->getNodeType(),
        ::jxx::org::w3c::dom::Node::COMMENT_NODE);
}

TEST(XmlDomLsParserConfigurationBehaviorStage, CommentsCanBeDisabled) {
    const auto value = parser();
    setFlag(value->getDomConfig(), "comments", false);
    const auto document = value->parse(input("<root><!--hidden--><child/></root>"));
    const auto root = document->getDocumentElement();
    ASSERT_NE(root, nullptr);
    ASSERT_NE(root->getFirstChild(), nullptr);
    EXPECT_EQ(root->getFirstChild()->getNodeName()->utf8(), "child");
    EXPECT_EQ(root->getFirstChild()->getNodeType(),
        ::jxx::org::w3c::dom::Node::ELEMENT_NODE);
}

TEST(XmlDomLsParserConfigurationBehaviorStage, NamespaceProcessingCanBeEnabled) {
    const auto value = parser();
    setFlag(value->getDomConfig(), "namespaces", true);
    const auto document = value->parse(input(
        "<p:root xmlns:p=\"urn:test\"><p:child/></p:root>"));
    const auto root = document->getDocumentElement();
    ASSERT_NE(root, nullptr);
    EXPECT_EQ(root->getNamespaceURI()->utf8(), "urn:test");
    EXPECT_EQ(root->getLocalName()->utf8(), "root");
}
