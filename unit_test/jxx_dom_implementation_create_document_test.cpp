#include <gtest/gtest.h>

#include "ext/xml/parsers/jxx.ext.xml.parsers.DocumentBuilder.h"
#include "ext/xml/parsers/jxx.ext.xml.parsers.DocumentBuilderFactory.h"
#include "lang/jxx.lang.String.h"
#include "org/w3c/dom/jxx.org.w3c.dom.DOMImplementation.h"
#include "org/w3c/dom/jxx.org.w3c.dom.Document.h"
#include "org/w3c/dom/jxx.org.w3c.dom.Element.h"

TEST(XmlDomImplementationStage, CreatesNamespacedDocument) {
    const auto builder =
        ::jxx::ext::xml::parsers::DocumentBuilderFactory::newInstance()
            ->newDocumentBuilder();

    const auto implementation = builder->getDOMImplementation();
    ASSERT_NE(implementation, nullptr);

    const auto document = implementation->createDocument(
        ::jxx::NEW<::jxx::lang::String>("urn:test"),
        ::jxx::NEW<::jxx::lang::String>("p:root"),
        nullptr);

    ASSERT_NE(document, nullptr);
    ASSERT_NE(document->getDocumentElement(), nullptr);
    EXPECT_EQ(document->getDocumentElement()->getTagName()->utf8(), "p:root");
    ASSERT_NE(document->getDocumentElement()->getNamespaceURI(), nullptr);
    EXPECT_EQ(document->getDocumentElement()->getNamespaceURI()->utf8(), "urn:test");
}

TEST(XmlDomImplementationStage, CreatesEmptyDocumentForEmptyName) {
    const auto builder =
        ::jxx::ext::xml::parsers::DocumentBuilderFactory::newInstance()
            ->newDocumentBuilder();

    const auto document = builder->getDOMImplementation()->createDocument(
        nullptr,
        ::jxx::NEW<::jxx::lang::String>(""),
        nullptr);

    ASSERT_NE(document, nullptr);
    EXPECT_EQ(document->getDocumentElement(), nullptr);
}
