#include <gtest/gtest.h>
#include "ext/xml/parsers/jxx.ext.xml.parsers.DocumentBuilderFactory.h"
#include "io/jxx.io.StringReader.h"
#include "lang/jxx.lang.String.h"
#include "org/w3c/dom/jxx.org.w3c.dom.Attr.h"
#include "org/w3c/dom/jxx.org.w3c.dom.Element.h"
#include "org/xml/sax/jxx.org.xml.sax.InputSource.h"
#include "ext/xml/parsers/jxx.ext.xml.parsers.DocumentBuilder.h"
#include "org/w3c/dom/jxx.org.w3c.dom.Document.h"
#include "org/w3c/dom/internal/jxx.org.w3c.dom.internal.PugiDom.h"
#include "org/w3c/dom/jxx.org.w3c.dom.NodeList.h"

TEST(XmlDomNamespaceAttribute, SupportsNamespaceLookupAndIdFlag) {
    auto factory = ::jxx::ext::xml::parsers::DocumentBuilderFactory::newInstance();
    factory->setNamespaceAware(true);
    auto builder = factory->newDocumentBuilder();
    auto reader = ::jxx::NEW<::jxx::io::StringReader>(::jxx::NEW<::jxx::lang::String>("<r xmlns:p='urn:test' p:id='42'><p:item/></r>"));
    auto document = builder->parse(::jxx::NEW<::jxx::org::xml::sax::InputSource>(reader));
    auto root = document->getDocumentElement();
    auto uri = ::jxx::NEW<::jxx::lang::String>("urn:test");
    auto id = ::jxx::NEW<::jxx::lang::String>("id");
    EXPECT_EQ(root->getAttributeNS(uri,id)->utf8(), "42");
    auto attribute = root->getAttributeNodeNS(uri,id);
    ASSERT_NE(attribute,nullptr);
    EXPECT_TRUE(attribute->getSpecified());
    EXPECT_EQ(attribute->getOwnerElement()->getTagName()->utf8(), "r");
    root->setIdAttributeNode(attribute,true);
    EXPECT_TRUE(attribute->isId());
    EXPECT_EQ(root->getElementsByTagNameNS(uri,::jxx::NEW<::jxx::lang::String>("item"))->getLength(),1);
}
