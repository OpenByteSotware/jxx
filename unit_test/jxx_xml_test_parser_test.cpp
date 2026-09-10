#include <gtest/gtest.h>

#include "ext/xml/parsers/jxx.ext.xml.parsers.DocumentBuilderFactory.h"
#include "io/jxx.io.ByteArrayInputStream.h"
#include "io/jxx.io.StringReader.h"
#include "lang/jxx.lang.String.h"
#include "org/w3c/dom/jxx.org.w3c.dom.Document.h"
#include "org/w3c/dom/jxx.org.w3c.dom.Element.h"
#include "org/xml/sax/jxx.org.xml.sax.InputSource.h"

TEST(XmlParserStage, ParsesCharacterStream) {
    auto factory = ::jxx::ext::xml::parsers::DocumentBuilderFactory::newInstance();
    auto builder = factory->newDocumentBuilder();
    auto reader = ::jxx::NEW<::jxx::io::StringReader>(
        ::jxx::NEW<::jxx::lang::String>("<root><item/></root>"));
    auto source = ::jxx::NEW<::jxx::org::xml::sax::InputSource>(reader);
    auto document = builder.get()->parse(source);
    ASSERT_NE(document, nullptr);
    ASSERT_NE(document->getDocumentElement(), nullptr);
    EXPECT_EQ(document->getDocumentElement()->getTagName()->utf8(), "root");
}

TEST(XmlParserStage, FactorySettingsReachBuilder) {
    auto factory = ::jxx::ext::xml::parsers::DocumentBuilderFactory::newInstance();
    factory->setNamespaceAware(true);
    factory->setIgnoringComments(true);
    auto builder = factory->newDocumentBuilder();
    EXPECT_TRUE(builder->isNamespaceAware());
    EXPECT_FALSE(builder->isValidating());
    EXPECT_FALSE(builder->isXIncludeAware());
    EXPECT_EQ(builder->getSchema(), nullptr);
}
