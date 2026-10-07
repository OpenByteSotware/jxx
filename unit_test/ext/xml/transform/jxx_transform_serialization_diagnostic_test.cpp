#include <gtest/gtest.h>

#include "ext/xml/parsers/jxx.ext.xml.parsers.DocumentBuilderFactory.h"
#include "ext/xml/transform/jxx.ext.xml.transform.OutputKeys.h"
#include "ext/xml/transform/jxx.ext.xml.transform.TransformerFactory.h"
#include "ext/xml/transform/dom/jxx.ext.xml.transform.dom.DOMSource.h"
#include "ext/xml/transform/stream/jxx.ext.xml.transform.stream.StreamResult.h"
#include "io/jxx.io.ByteArrayOutputStream.h"
#include "lang/jxx.lang.String.h"
#include "org/w3c/dom/jxx.org.w3c.dom.Document.h"
#include "org/w3c/dom/jxx.org.w3c.dom.Node.h"
#include "ext/xml/parsers/jxx.ext.xml.parsers.DocumentBuilder.h"
#include "org/w3c/dom/jxx.org.w3c.dom.Text.h"
#include "org/w3c/dom/jxx.org.w3c.dom.Element.h"
#include "io/jxx.io.ByteArrayOutputStream.h"
namespace {

::jxx::Ptr<::jxx::lang::String> text(const char* value)
{
    return ::jxx::NEW<::jxx::lang::String>(value);
}

::jxx::Ptr<::jxx::org::w3c::dom::Document> documentWithRoot(
    bool includeText)
{
    const auto factory =
        ::jxx::ext::xml::parsers::DocumentBuilderFactory::newInstance();
    if (!factory) return nullptr;
    const auto builder = factory->newDocumentBuilder();
    if (!builder) return nullptr;
    const auto document = builder->newDocument();
    if (!document) return nullptr;
    const auto root = document->createElement(text("DeviceConfig"));
    if (!root) return nullptr;
    if (includeText) root->setTextContent(text("new-value"));
    document->appendChild(
        ::jxx::CAST<::jxx::org::w3c::dom::Node>(root));
    return document;
}

TEST(JxxTransformSerializationDiagnosticTest, CreatesTransformer)
{
    const auto factory =
        ::jxx::ext::xml::transform::TransformerFactory::newInstance();
    ASSERT_NE(nullptr, factory);
    const auto transformer = factory->newTransformer();
    ASSERT_NE(nullptr, transformer);
}

TEST(JxxTransformSerializationDiagnosticTest, CreatesAndCastsSource)
{
    const auto document = documentWithRoot(false);
    ASSERT_NE(nullptr, document);
    const auto source =
        ::jxx::NEW<::jxx::ext::xml::transform::dom::DOMSource>(
            ::jxx::CAST<::jxx::org::w3c::dom::Node>(document));
    ASSERT_NE(nullptr, source);
    ASSERT_NE(nullptr, source->getNode());
    EXPECT_NE(nullptr,
        ::jxx::CAST<::jxx::ext::xml::transform::Source>(source));
}

TEST(JxxTransformSerializationDiagnosticTest, CreatesAndCastsResult)
{
    const auto output = ::jxx::NEW<::jxx::io::ByteArrayOutputStream>();
    ASSERT_NE(nullptr, output);
    const auto outputStream =
        ::jxx::CAST<::jxx::io::OutputStream>(output);
    ASSERT_NE(nullptr, outputStream);
    const auto result =
        ::jxx::NEW<::jxx::ext::xml::transform::stream::StreamResult>(
            outputStream);
    ASSERT_NE(nullptr, result);
    ASSERT_EQ(outputStream, result->getOutputStream());
    EXPECT_NE(nullptr,
        ::jxx::CAST<::jxx::ext::xml::transform::Result>(result));
}

TEST(JxxTransformSerializationDiagnosticTest, ByteArrayOutputStreamRoundTrip)
{
    const auto output = ::jxx::NEW<::jxx::io::ByteArrayOutputStream>();
    const auto bytes = text("probe")->getBytes(text("UTF-8"));
    ASSERT_NE(nullptr, bytes);
    output->write( bytes, 0,  static_cast<::jxx::lang::jint>(bytes->length));
    const auto value = output->toString();
    ASSERT_NE(nullptr, value);
    EXPECT_EQ("probe", value->utf8());
}

TEST(JxxTransformSerializationDiagnosticTest, TransformsRootWithoutText)
{
    const auto document = documentWithRoot(false);
    ASSERT_NE(nullptr, document);
    const auto transformer =
        ::jxx::ext::xml::transform::TransformerFactory::newInstance()
            ->newTransformer();
    ASSERT_NE(nullptr, transformer);
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
}

TEST(JxxTransformSerializationDiagnosticTest, TransformsRootWithText)
{
    const auto document = documentWithRoot(true);
    ASSERT_NE(nullptr, document);
    const auto transformer =
        ::jxx::ext::xml::transform::TransformerFactory::newInstance()
            ->newTransformer();
    ASSERT_NE(nullptr, transformer);
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
              serialized->utf8().find("new-value"));
}

TEST(JxxTransformSerializationDiagnosticTest, SettingEncodingDoesNotCrash)
{
    const auto transformer =
        ::jxx::ext::xml::transform::TransformerFactory::newInstance()
            ->newTransformer();
    ASSERT_NE(nullptr, transformer);
    transformer->setOutputProperty(
        ::jxx::ext::xml::transform::OutputKeys::ENCODING(),
        text("UTF-8"));
    const auto encoding = transformer->getOutputProperty(
        ::jxx::ext::xml::transform::OutputKeys::ENCODING());
    ASSERT_NE(nullptr, encoding);
    EXPECT_EQ("UTF-8", encoding->utf8());
}

} // namespace
