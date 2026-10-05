#include <gtest/gtest.h>

#include "ext/xml/parsers/jxx.ext.xml.parsers.DocumentBuilderFactory.h"
#include "ext/xml/transform/dom/jxx.ext.xml.transform.dom.DOMSource.h"
#include "ext/xml/transform/jxx.ext.xml.transform.OutputKeys.h"
#include "ext/xml/transform/jxx.ext.xml.transform.TransformerFactory.h"
#include "ext/xml/transform/stream/jxx.ext.xml.transform.stream.StreamResult.h"
#include "ext/xml/xpath/jxx.ext.xml.xpath.XPathConstants.h"
#include "ext/xml/xpath/jxx.ext.xml.xpath.XPathFactory.h"
#include "io/jxx.io.ByteArrayOutputStream.h"
#include "lang/jxx.lang.String.h"
#include "lang/jxx.lang.Object.h"
#include "org/w3c/dom/jxx.org.w3c.dom.Document.h"
#include "org/w3c/dom/jxx.org.w3c.dom.Element.h"
#include "org/w3c/dom/jxx.org.w3c.dom.Node.h"
#include "org/w3c/dom/jxx.org.w3c.dom.NodeList.h"
#include "ext/xml/parsers/jxx.ext.xml.parsers.DocumentBuilder.h"
#include "org/w3c/dom/jxx.org.w3c.dom.Text.h"

namespace {

::jxx::Ptr<::jxx::lang::String> text(const char* value) {
    return ::jxx::NEW<::jxx::lang::String>(value);
}

} // namespace

TEST(JxxInputRouterXmlPipelineTest, CreatesQueriesMutatesAndSerializesDocument) {
    const auto factory =
        ::jxx::ext::xml::parsers::DocumentBuilderFactory::newInstance();
    ASSERT_NE(nullptr, factory);
    factory->setNamespaceAware(true);
    EXPECT_TRUE(factory->isNamespaceAware());

    const auto builder = factory->newDocumentBuilder();
    ASSERT_NE(nullptr, builder);
    const auto document = builder->newDocument();
    ASSERT_NE(nullptr, document);

    const auto root = document->createElement(text("DeviceConfig"));
    const auto hardware = document->createElement(text("HardwareConfig"));
    const auto configuration = document->createElement(text("ConfigID"));
    ASSERT_NE(nullptr, root);
    ASSERT_NE(nullptr, hardware);
    ASSERT_NE(nullptr, configuration);

    configuration->appendChild(document->createTextNode(text("old-value")));
    hardware->appendChild(::jxx::CAST<::jxx::org::w3c::dom::Node>(configuration));
    root->appendChild(::jxx::CAST<::jxx::org::w3c::dom::Node>(hardware));
    document->appendChild(::jxx::CAST<::jxx::org::w3c::dom::Node>(root));

    const auto xpathFactory =
        ::jxx::ext::xml::xpath::XPathFactory::newInstance();
    ASSERT_NE(nullptr, xpathFactory);
    const auto xpath = xpathFactory->newXPath();
    ASSERT_NE(nullptr, xpath);
    const auto expression = xpath->compile(
        text("//DeviceConfig/HardwareConfig/ConfigID"));
    ASSERT_NE(nullptr, expression);

    const auto evaluated = expression->evaluate(
        ::jxx::CAST<::jxx::lang::Object>(document),
        ::jxx::ext::xml::xpath::XPathConstants::NODESET());
    const auto nodes = ::jxx::CAST<::jxx::org::w3c::dom::NodeList>(evaluated);
    ASSERT_NE(nullptr, nodes);
    ASSERT_EQ(1, nodes->getLength());
    const auto matched = nodes->item(0);
    ASSERT_NE(nullptr, matched);
    ASSERT_NE(nullptr, matched->getTextContent());
    EXPECT_EQ("old-value", matched->getTextContent()->utf8());

    matched->setTextContent(text("new-value"));
    ASSERT_NE(nullptr, matched->getTextContent());
    EXPECT_EQ("new-value", matched->getTextContent()->utf8());

    const auto transformerFactory =
        ::jxx::ext::xml::transform::TransformerFactory::newInstance();
    ASSERT_NE(nullptr, transformerFactory);
    const auto transformer = transformerFactory->newTransformer();
    ASSERT_NE(nullptr, transformer);
    transformer->setOutputProperty(
        ::jxx::ext::xml::transform::OutputKeys::INDENT(), text("yes"));
    transformer->setOutputProperty(
        ::jxx::ext::xml::transform::OutputKeys::ENCODING(), text("UTF-8"));
    transformer->setOutputProperty(
        ::jxx::ext::xml::transform::OutputKeys::STANDALONE(), text("no"));
    transformer->setOutputProperty(
        ::jxx::ext::xml::transform::OutputKeys::METHOD(), text("xml"));

    const auto output = ::jxx::NEW<::jxx::io::ByteArrayOutputStream>();
    const auto source =
        ::jxx::NEW<::jxx::ext::xml::transform::dom::DOMSource>(
            ::jxx::CAST<::jxx::org::w3c::dom::Node>(document));
    const auto result =
        ::jxx::NEW<::jxx::ext::xml::transform::stream::StreamResult>(
            ::jxx::CAST<::jxx::io::OutputStream>(output));

    transformer->transform(
        ::jxx::CAST<::jxx::ext::xml::transform::Source>(source),
        ::jxx::CAST<::jxx::ext::xml::transform::Result>(result));

    const auto serialized = output->toString();
    ASSERT_NE(nullptr, serialized);
    EXPECT_NE(std::string::npos, serialized->utf8().find("new-value"));
    EXPECT_NE(std::string::npos, serialized->utf8().find("DeviceConfig"));
}

TEST(JxxInputRouterXmlPipelineTest, EmptyNodeSetIsNotNull) {
    const auto factory =
        ::jxx::ext::xml::parsers::DocumentBuilderFactory::newInstance();
    const auto builder = factory->newDocumentBuilder();
    const auto document = builder->newDocument();
    document->appendChild(::jxx::CAST<::jxx::org::w3c::dom::Node>(
        document->createElement(text("DeviceConfig"))));

    const auto xpath =
        ::jxx::ext::xml::xpath::XPathFactory::newInstance()->newXPath();
    const auto result = xpath->evaluate(
        text("//Missing"),
        ::jxx::CAST<::jxx::lang::Object>(document),
        ::jxx::ext::xml::xpath::XPathConstants::NODESET());
    const auto nodes = ::jxx::CAST<::jxx::org::w3c::dom::NodeList>(result);

    ASSERT_NE(nullptr, nodes);
    EXPECT_EQ(0, nodes->getLength());
}
