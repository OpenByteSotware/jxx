#include <gtest/gtest.h>
#include "ext/xml/transform/dom/jxx.ext.xml.transform.dom.DOMSource.h"
#include "ext/xml/transform/jxx.ext.xml.transform.OutputKeys.h"
#include "ext/xml/transform/jxx.ext.xml.transform.TransformerFactory.h"
#include "ext/xml/transform/stream/jxx.ext.xml.transform.stream.StreamResult.h"
#include "io/jxx.io.ByteArrayOutputStream.h"
#include "io/jxx.io.OutputStreamWriter.h"
#include "org/w3c/dom/internal/jxx.org.w3c.dom.internal.PugiDom.h"

TEST(XmlTransformCoreTest, IdentityTransformWritesDocument) {
    std::string error;
    ::jxx::lang::jint offset = 0;
    const auto document = ::jxx::org::w3c::dom::internal::parseDocument(
        "<root value=\"1\"><child>text</child></root>",
        false,
        false,
        error,
        offset);
    ASSERT_NE(nullptr, document);

    const auto factory = ::jxx::ext::xml::transform::TransformerFactory::newInstance();
    const auto transformer = factory->newTransformer();
    transformer->setOutputProperty(
        ::jxx::ext::xml::transform::OutputKeys::OMIT_XML_DECLARATION(),
        ::jxx::NEW<::jxx::lang::String>("yes"));

    const auto bytes = ::jxx::NEW<::jxx::io::ByteArrayOutputStream>();
    const auto writer = ::jxx::NEW<::jxx::io::OutputStreamWriter>(bytes);
    transformer->transform(
        ::jxx::NEW<::jxx::ext::xml::transform::dom::DOMSource>(document),
        ::jxx::NEW<::jxx::ext::xml::transform::stream::StreamResult>(writer));

    const auto result = bytes->toString()->utf8();
    EXPECT_NE(std::string::npos, result.find("<root value=\"1\">"));
    EXPECT_NE(std::string::npos, result.find("<child>text</child>"));
}

TEST(XmlTransformCoreTest, OutputPropertiesRoundTrip) {
    const auto transformer =
        ::jxx::ext::xml::transform::TransformerFactory::newInstance()->newTransformer();
    transformer->setOutputProperty(
        ::jxx::ext::xml::transform::OutputKeys::INDENT(),
        ::jxx::NEW<::jxx::lang::String>("yes"));
    EXPECT_EQ(
        "yes",
        transformer->getOutputProperty(
            ::jxx::ext::xml::transform::OutputKeys::INDENT())->utf8());
}
