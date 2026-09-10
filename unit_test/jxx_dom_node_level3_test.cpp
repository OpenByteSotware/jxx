#include <gtest/gtest.h>

#include "ext/xml/parsers/jxx.ext.xml.parsers.DocumentBuilder.h"
#include "ext/xml/parsers/jxx.ext.xml.parsers.DocumentBuilderFactory.h"
#include "io/jxx.io.StringReader.h"
#include "lang/jxx.lang.String.h"
#include "org/w3c/dom/jxx.org.w3c.dom.Document.h"
#include "org/w3c/dom/jxx.org.w3c.dom.Element.h"
#include "org/xml/sax/jxx.org.xml.sax.InputSource.h"

TEST(XmlDomNodeLevel3, ResolvesNamespacesAndComparesIdentity) {
    const auto builder = ::jxx::ext::xml::parsers::DocumentBuilderFactory::newInstance()->newDocumentBuilder();
    const auto reader = ::jxx::NEW<::jxx::io::StringReader>(
        ::jxx::NEW<::jxx::lang::String>("<r xmlns='urn:default' xmlns:p='urn:test'><p:item/></r>"));
    const auto document = builder->parse(
        ::jxx::NEW<::jxx::org::xml::sax::InputSource>(reader));
    const auto root = document->getDocumentElement();
    ASSERT_NE(root, nullptr);
    EXPECT_TRUE(root->isSameNode(root));
    EXPECT_TRUE(root->isEqualNode(root));
    EXPECT_EQ(root->lookupPrefix(::jxx::NEW<::jxx::lang::String>("urn:test"))->utf8(), "p");
    EXPECT_EQ(root->lookupNamespaceURI(::jxx::NEW<::jxx::lang::String>("p"))->utf8(), "urn:test");
    EXPECT_TRUE(root->isDefaultNamespace(::jxx::NEW<::jxx::lang::String>("urn:default")));
}
