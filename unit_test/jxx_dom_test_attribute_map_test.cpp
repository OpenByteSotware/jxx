#include <gtest/gtest.h>

#include "ext/xml/parsers/jxx.ext.xml.parsers.DocumentBuilderFactory.h"
#include "io/jxx.io.StringReader.h"
#include "lang/jxx.lang.String.h"
#include "org/w3c/dom/jxx.org.w3c.dom.Attr.h"
#include "org/w3c/dom/jxx.org.w3c.dom.Document.h"
#include "org/w3c/dom/jxx.org.w3c.dom.Element.h"
#include "org/w3c/dom/jxx.org.w3c.dom.NamedNodeMap.h"
#include "org/w3c/dom/jxx.org.w3c.dom.Node.h"
#include "org/xml/sax/jxx.org.xml.sax.InputSource.h"
#include "ext/xml/parsers/jxx.ext.xml.parsers.DocumentBuilder.h"

TEST(XmlDomAttributeStage, NamedAttributeLookupAndIndexing) {
    auto factory = ::jxx::ext::xml::parsers::DocumentBuilderFactory::newInstance();
    auto builder = factory->newDocumentBuilder();
    auto reader = ::jxx::NEW<::jxx::io::StringReader>(
        ::jxx::NEW<::jxx::lang::String>("<root first=\"1\" second=\"2\"/>"));
    auto document = builder->parse(
        ::jxx::NEW<::jxx::org::xml::sax::InputSource>(reader));
    auto attributes = document->getDocumentElement()->getAttributes();
    ASSERT_NE(attributes, nullptr);
    EXPECT_EQ(attributes->getLength(), 2);
    auto first = attributes->getNamedItem(
        ::jxx::NEW<::jxx::lang::String>("first"));
    ASSERT_NE(first, nullptr);
    EXPECT_EQ(first->getNodeType(), ::jxx::org::w3c::dom::Node::ATTRIBUTE_NODE);
    EXPECT_EQ(first->getNodeName()->utf8(), "first");
    EXPECT_EQ(first->getNodeValue()->utf8(), "1");
    EXPECT_NE(attributes->item(0), nullptr);
    EXPECT_EQ(attributes->item(2), nullptr);
}

TEST(XmlDomAttributeStage, RemoveReturnsAttribute) {
    auto factory = ::jxx::ext::xml::parsers::DocumentBuilderFactory::newInstance();
    auto builder = factory->newDocumentBuilder();
    auto reader = ::jxx::NEW<::jxx::io::StringReader>(
        ::jxx::NEW<::jxx::lang::String>("<root value=\"old\"/>"));
    auto document = builder->parse(
        ::jxx::NEW<::jxx::org::xml::sax::InputSource>(reader));
    auto attributes = document->getDocumentElement()->getAttributes();
    auto removed = attributes->removeNamedItem(
        ::jxx::NEW<::jxx::lang::String>("value"));
    ASSERT_NE(removed, nullptr);
    EXPECT_EQ(removed->getNodeValue()->utf8(), "old");
    EXPECT_EQ(attributes->getLength(), 0);
}
