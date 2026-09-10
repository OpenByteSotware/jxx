#include <gtest/gtest.h>

#include "ext/xml/parsers/jxx.ext.xml.parsers.DocumentBuilder.h"
#include "ext/xml/parsers/jxx.ext.xml.parsers.DocumentBuilderFactory.h"
#include "lang/jxx.lang.String.h"
#include "org/w3c/dom/jxx.org.w3c.dom.Document.h"
#include "org/w3c/dom/jxx.org.w3c.dom.Element.h"
#include "org/w3c/dom/jxx.org.w3c.dom.NodeList.h"

TEST(XmlDomDocumentStage, NamespaceFactoriesAndMetadata) {
    auto builder = ::jxx::ext::xml::parsers::DocumentBuilderFactory::newInstance()->newDocumentBuilder();
    auto document = builder->newDocument();
    ASSERT_NE(document, nullptr);
    auto uri = ::jxx::NEW<::jxx::lang::String>("urn:test");
    auto name = ::jxx::NEW<::jxx::lang::String>("p:root");
    auto root = document->createElementNS(uri, name);
    ASSERT_NE(root, nullptr);
    document->appendChild(root);
    EXPECT_EQ(document->getDocumentElement()->getTagName()->utf8(), "p:root");
    document->setDocumentURI(::jxx::NEW<::jxx::lang::String>("memory:test"));
    EXPECT_EQ(document->getDocumentURI()->utf8(), "memory:test");
    document->setXmlStandalone(true);
    EXPECT_TRUE(document->getXmlStandalone());
    EXPECT_EQ(document->getXmlVersion()->utf8(), "1.0");
    EXPECT_EQ(document->getElementsByTagNameNS(uri, ::jxx::NEW<::jxx::lang::String>("root"))->getLength(), 1);
}
