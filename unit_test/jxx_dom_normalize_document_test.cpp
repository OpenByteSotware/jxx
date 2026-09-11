#include <gtest/gtest.h>

#include "ext/xml/parsers/jxx.ext.xml.parsers.DocumentBuilder.h"
#include "ext/xml/parsers/jxx.ext.xml.parsers.DocumentBuilderFactory.h"
#include "io/jxx.io.StringReader.h"
#include "lang/jxx.lang.Boolean.h"
#include "lang/jxx.lang.String.h"
#include "org/w3c/dom/jxx.org.w3c.dom.DOMConfiguration.h"
#include "org/w3c/dom/jxx.org.w3c.dom.Document.h"
#include "org/w3c/dom/jxx.org.w3c.dom.Node.h"
#include "org/w3c/dom/jxx.org.w3c.dom.NodeList.h"
#include "org/xml/sax/jxx.org.xml.sax.InputSource.h"
#include "org/w3c/dom/jxx.org.w3c.dom.Element.h"

namespace {

::jxx::Ptr<::jxx::org::w3c::dom::Document> parseDocument(
    const char* xml) {
    const auto builder =
        ::jxx::ext::xml::parsers::DocumentBuilderFactory::newInstance()
            ->newDocumentBuilder();

    return builder->parse(
        ::jxx::NEW<::jxx::org::xml::sax::InputSource>(
            ::jxx::NEW<::jxx::io::StringReader>(
                ::jxx::NEW<::jxx::lang::String>(xml))));
}

void setBooleanParameter(
    const ::jxx::Ptr<::jxx::org::w3c::dom::DOMConfiguration>& configuration,
    const char* name,
    ::jxx::lang::jbool value) {
    configuration->setParameter(
        ::jxx::NEW<::jxx::lang::String>(name),
        ::jxx::CAST<::jxx::lang::Object>(
            ::jxx::lang::Boolean::valueOf(value)));
}

} // namespace

TEST(XmlDomNormalizeDocumentStage, RemovesCommentsWhenDisabled) {
    const auto document = parseDocument(
        "<root><!--one--><child/><!--two--></root>");

    setBooleanParameter(
        document->getDomConfig(),
        "comments",
        false);

    document->normalizeDocument();

    const auto children = document->getDocumentElement()->getChildNodes();
    ASSERT_EQ(children->getLength(), 1);
    EXPECT_EQ(
        children->item(0)->getNodeType(),
        ::jxx::org::w3c::dom::Node::ELEMENT_NODE);
}

TEST(XmlDomNormalizeDocumentStage, ConvertsAndMergesCdataWhenDisabled) {
    const auto document = parseDocument(
        "<root>one<![CDATA[two]]>three</root>");

    setBooleanParameter(
        document->getDomConfig(),
        "cdata-sections",
        false);

    document->normalizeDocument();

    const auto children = document->getDocumentElement()->getChildNodes();
    ASSERT_EQ(children->getLength(), 1);
    EXPECT_EQ(
        children->item(0)->getNodeType(),
        ::jxx::org::w3c::dom::Node::TEXT_NODE);
    EXPECT_EQ(children->item(0)->getNodeValue()->utf8(), "onetwothree");
}

TEST(XmlDomNormalizeDocumentStage, PreservesCdataByDefault) {
    const auto document = parseDocument(
        "<root><![CDATA[value]]></root>");

    document->normalizeDocument();

    const auto child =
        document->getDocumentElement()->getFirstChild();
    ASSERT_NE(child, nullptr);
    EXPECT_EQ(
        child->getNodeType(),
        ::jxx::org::w3c::dom::Node::CDATA_SECTION_NODE);
}
