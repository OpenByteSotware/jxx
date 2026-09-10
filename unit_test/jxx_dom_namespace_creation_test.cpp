#include <gtest/gtest.h>

#include "ext/xml/parsers/jxx.ext.xml.parsers.DocumentBuilder.h"
#include "ext/xml/parsers/jxx.ext.xml.parsers.DocumentBuilderFactory.h"
#include "lang/jxx.lang.String.h"
#include "org/w3c/dom/jxx.org.w3c.dom.Document.h"
#include "org/w3c/dom/jxx.org.w3c.dom.Element.h"
#include "org/w3c/dom/jxx.org.w3c.dom.NodeList.h"

TEST(XmlDomNamespaceCreation, CreatedElementRetainsNamespaceAssociation) {
    const auto builder =
        ::jxx::ext::xml::parsers::DocumentBuilderFactory::newInstance()
            ->newDocumentBuilder();
    const auto document = builder->newDocument();
    const auto uri = ::jxx::NEW<::jxx::lang::String>("urn:test");
    const auto qualifiedName =
        ::jxx::NEW<::jxx::lang::String>("p:root");

    const auto root = document->createElementNS(uri, qualifiedName);
    document->appendChild(root);

    ASSERT_NE(document->getDocumentElement(), nullptr);
    ASSERT_NE(document->getDocumentElement()->getNamespaceURI(), nullptr);
    EXPECT_EQ(document->getDocumentElement()->getNamespaceURI()->utf8(), "urn:test");

    const auto matches = document->getElementsByTagNameNS(
        uri,
        ::jxx::NEW<::jxx::lang::String>("root"));
    ASSERT_NE(matches, nullptr);
    EXPECT_EQ(matches->getLength(), 1);
}

TEST(XmlDomNamespaceCreation, NamespacedAttributeRetainsPrefixDeclaration) {
    const auto builder =
        ::jxx::ext::xml::parsers::DocumentBuilderFactory::newInstance()
            ->newDocumentBuilder();
    const auto document = builder->newDocument();
    const auto element = document->createElement(
        ::jxx::NEW<::jxx::lang::String>("root"));
    document->appendChild(element);

    const auto uri = ::jxx::NEW<::jxx::lang::String>("urn:attribute");
    element->setAttributeNS(
        uri,
        ::jxx::NEW<::jxx::lang::String>("a:id"),
        ::jxx::NEW<::jxx::lang::String>("42"));

    EXPECT_EQ(
        element->getAttributeNS(
            uri,
            ::jxx::NEW<::jxx::lang::String>("id"))->utf8(),
        "42");
}
