#include <gtest/gtest.h>

#include "ext/xml/parsers/jxx.ext.xml.parsers.DocumentBuilderFactory.h"
#include "ext/xml/transform/jxx.ext.xml.transform.OutputKeys.h"
#include "ext/xml/transform/jxx.ext.xml.transform.TransformerFactory.h"
#include "ext/xml/transform/dom/jxx.ext.xml.transform.dom.DOMSource.h"
#include "ext/xml/transform/stream/jxx.ext.xml.transform.stream.StreamResult.h"
#include "ext/xml/xpath/jxx.ext.xml.xpath.XPathConstants.h"
#include "ext/xml/xpath/jxx.ext.xml.xpath.XPathFactory.h"
#include "io/jxx.io.ByteArrayOutputStream.h"
#include "lang/jxx.lang.String.h"
#include "org/w3c/dom/jxx.org.w3c.dom.Document.h"
#include "org/w3c/dom/jxx.org.w3c.dom.Element.h"
#include "org/w3c/dom/jxx.org.w3c.dom.Node.h"
#include "org/w3c/dom/jxx.org.w3c.dom.NodeList.h"
#include "ext/xml/parsers/jxx.ext.xml.parsers.DocumentBuilder.h"
#include "org/w3c/dom/jxx.org.w3c.dom.Text.h"
namespace {

::jxx::Ptr<::jxx::lang::String> text(const char* value)
{
    return ::jxx::NEW<::jxx::lang::String>(value);
}

::jxx::Ptr<::jxx::org::w3c::dom::Document> newDocument()
{
    const auto factory =
        ::jxx::ext::xml::parsers::DocumentBuilderFactory::newInstance();
    EXPECT_NE(nullptr, factory);
    if (!factory) return nullptr;

    const auto builder = factory->newDocumentBuilder();
    EXPECT_NE(nullptr, builder);
    return builder ? builder->newDocument() : nullptr;
}

TEST(JxxXmlPipelineDiagnosticTest, CreatesDocument)
{
    const auto document = newDocument();
    ASSERT_NE(nullptr, document);
}

TEST(JxxXmlPipelineDiagnosticTest, CreatesElement)
{
    const auto document = newDocument();
    ASSERT_NE(nullptr, document);

    const auto root = document->createElement(text("DeviceConfig"));
    ASSERT_NE(nullptr, root);
    ASSERT_NE(nullptr, root->getNodeName());
    EXPECT_EQ("DeviceConfig", root->getNodeName()->utf8());
}

TEST(JxxXmlPipelineDiagnosticTest, CreatesDetachedTextNode)
{
    const auto document = newDocument();
    ASSERT_NE(nullptr, document);

    const auto value = document->createTextNode(text("old-value"));
    ASSERT_NE(nullptr, value);
    ASSERT_NE(nullptr, value->getNodeValue());
    EXPECT_EQ("old-value", value->getNodeValue()->utf8());
}

TEST(JxxXmlPipelineDiagnosticTest, AppendsTextToElement)
{
    const auto document = newDocument();
    ASSERT_NE(nullptr, document);

    const auto configuration = document->createElement(text("ConfigID"));
    ASSERT_NE(nullptr, configuration);
    const auto value = document->createTextNode(text("old-value"));
    ASSERT_NE(nullptr, value);

    const auto appended = configuration->appendChild(
        ::jxx::CAST<::jxx::org::w3c::dom::Node>(value));
    ASSERT_NE(nullptr, appended);
    ASSERT_NE(nullptr, configuration->getTextContent());
    EXPECT_EQ("old-value", configuration->getTextContent()->utf8());
}

TEST(JxxXmlPipelineDiagnosticTest, BuildsNestedDocument)
{
    const auto document = newDocument();
    ASSERT_NE(nullptr, document);

    const auto root = document->createElement(text("DeviceConfig"));
    const auto hardware = document->createElement(text("HardwareConfig"));
    const auto configuration = document->createElement(text("ConfigID"));
    ASSERT_NE(nullptr, root);
    ASSERT_NE(nullptr, hardware);
    ASSERT_NE(nullptr, configuration);

    ASSERT_NE(nullptr, configuration->appendChild(
        ::jxx::CAST<::jxx::org::w3c::dom::Node>(
            document->createTextNode(text("old-value")))));
    ASSERT_NE(nullptr, hardware->appendChild(
        ::jxx::CAST<::jxx::org::w3c::dom::Node>(configuration)));
    ASSERT_NE(nullptr, root->appendChild(
        ::jxx::CAST<::jxx::org::w3c::dom::Node>(hardware)));
    ASSERT_NE(nullptr, document->appendChild(
        ::jxx::CAST<::jxx::org::w3c::dom::Node>(root)));

    ASSERT_NE(nullptr, document->getDocumentElement());
    EXPECT_EQ("DeviceConfig",
              document->getDocumentElement()->getNodeName()->utf8());
}

TEST(JxxXmlPipelineDiagnosticTest, EvaluatesDescendantPath)
{
    const auto document = newDocument();
    ASSERT_NE(nullptr, document);

    const auto root = document->createElement(text("DeviceConfig"));
    const auto hardware = document->createElement(text("HardwareConfig"));
    const auto configuration = document->createElement(text("ConfigID"));
    configuration->appendChild(::jxx::CAST<::jxx::org::w3c::dom::Node>(
        document->createTextNode(text("old-value"))));
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
    ASSERT_NE(nullptr, evaluated);
    const auto nodes =
        ::jxx::CAST<::jxx::org::w3c::dom::NodeList>(evaluated);
    ASSERT_NE(nullptr, nodes);
    ASSERT_EQ(1, nodes->getLength());
    ASSERT_NE(nullptr, nodes->item(0));
    EXPECT_EQ("old-value", nodes->item(0)->getTextContent()->utf8());
}

TEST(JxxXmlPipelineDiagnosticTest, MutatesMatchedText)
{
    const auto document = newDocument();
    ASSERT_NE(nullptr, document);
    const auto configuration = document->createElement(text("ConfigID"));
    ASSERT_NE(nullptr, configuration);
    configuration->appendChild(::jxx::CAST<::jxx::org::w3c::dom::Node>(
        document->createTextNode(text("old-value"))));

    configuration->setTextContent(text("new-value"));
    ASSERT_NE(nullptr, configuration->getTextContent());
    EXPECT_EQ("new-value", configuration->getTextContent()->utf8());
}

TEST(JxxXmlPipelineDiagnosticTest, SerializesSimpleDocument)
{
    const auto document = newDocument();
    ASSERT_NE(nullptr, document);
    const auto root = document->createElement(text("DeviceConfig"));
    ASSERT_NE(nullptr, root);
    root->setTextContent(text("new-value"));
    document->appendChild(::jxx::CAST<::jxx::org::w3c::dom::Node>(root));

    const auto factory =
        ::jxx::ext::xml::transform::TransformerFactory::newInstance();
    ASSERT_NE(nullptr, factory);
    const auto transformer = factory->newTransformer();
    ASSERT_NE(nullptr, transformer);
    transformer->setOutputProperty(
        ::jxx::ext::xml::transform::OutputKeys::ENCODING(), text("UTF-8"));

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
    EXPECT_NE(std::string::npos,
              serialized->utf8().find("DeviceConfig"));
    EXPECT_NE(std::string::npos,
              serialized->utf8().find("new-value"));
}

} // namespace
