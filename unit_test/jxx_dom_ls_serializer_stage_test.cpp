#include <gtest/gtest.h>

#include "lang/jxx.lang.String.h"
#include "org/w3c/dom/internal/jxx.org.w3c.dom.internal.DOMImplementationImpl.h"
#include "org/w3c/dom/jxx.org.w3c.dom.Document.h"
#include "org/w3c/dom/jxx.org.w3c.dom.Element.h"
#include "org/w3c/dom/jxx.org.w3c.dom.Node.h"
#include "org/w3c/dom/jxx.org.w3c.dom.Text.h"
#include "org/w3c/dom/ls/internal/jxx.org.w3c.dom.ls.internal.DOMImplementationLSImpl.h"
#include "org/w3c/dom/ls/jxx.org.w3c.dom.ls.LSSerializer.h"

TEST(XmlDomLsSerializerStage, FactoryCreatesSerializer) {
    const auto serializer =
        ::jxx::org::w3c::dom::ls::internal::loadSaveImplementation()->createLSSerializer();
    EXPECT_NE(serializer, nullptr);
}

TEST(XmlDomLsSerializerStage, SerializesElementAttributesAndEscapedText) {
    const auto core = ::jxx::org::w3c::dom::internal::implementation();
    const auto document = core->createDocument(
        nullptr,
        ::jxx::NEW<::jxx::lang::String>("root"),
        nullptr);
    const auto root = document->getDocumentElement();
    root->setAttribute(
        ::jxx::NEW<::jxx::lang::String>("id"),
        ::jxx::NEW<::jxx::lang::String>("A&B"));
    const auto text = document->createTextNode(
        ::jxx::NEW<::jxx::lang::String>("x < y"));
    root->appendChild(
        ::jxx::CAST<::jxx::org::w3c::dom::Node>(text));

    const auto serializer =
        ::jxx::org::w3c::dom::ls::internal::loadSaveImplementation()->createLSSerializer();
    const auto result = serializer->writeToString(
        ::jxx::CAST<::jxx::org::w3c::dom::Node>(document));

    ASSERT_NE(result, nullptr);
    EXPECT_EQ(result->utf8(),"<?xml version=\"1.0\" encoding=\"UTF-8\"?>""<root id=\"A&amp;B\">x &lt; y</root>");
}

TEST(XmlDomLsSerializerStage, RetainsNewLineProperty) {
    const auto serializer =
        ::jxx::org::w3c::dom::ls::internal::loadSaveImplementation()->createLSSerializer();
    const auto value = ::jxx::NEW<::jxx::lang::String>("\r\n");
    serializer->setNewLine(value);
    EXPECT_EQ(serializer->getNewLine(), value);
}
