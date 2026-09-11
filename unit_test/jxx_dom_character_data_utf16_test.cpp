#include <gtest/gtest.h>

#include "ext/xml/parsers/jxx.ext.xml.parsers.DocumentBuilder.h"
#include "ext/xml/parsers/jxx.ext.xml.parsers.DocumentBuilderFactory.h"
#include "io/jxx.io.StringReader.h"
#include "lang/jxx.lang.String.h"
#include "org/w3c/dom/jxx.org.w3c.dom.Document.h"
#include "org/w3c/dom/jxx.org.w3c.dom.Element.h"
#include "org/w3c/dom/jxx.org.w3c.dom.Node.h"
#include "org/w3c/dom/jxx.org.w3c.dom.Text.h"
#include "org/xml/sax/jxx.org.xml.sax.InputSource.h"

namespace {

::jxx::Ptr<::jxx::org::w3c::dom::Document>
parseUtf16Data() {
    const auto factory =
        ::jxx::ext::xml::parsers::
            DocumentBuilderFactory::newInstance();

    const auto builder =
        factory->newDocumentBuilder();

    const auto reader =
        ::jxx::NEW<::jxx::io::StringReader>(
            ::jxx::NEW<::jxx::lang::String>(
                u"<root>A\U0001F600B</root>"));

    return builder->parse(
        ::jxx::NEW<
            ::jxx::org::xml::sax::InputSource>(
                reader));
}

} // namespace

TEST(
    XmlDomCharacterDataUtf16Stage,
    LengthUsesUtf16CodeUnits) {
    const auto document = parseUtf16Data();
    ASSERT_NE(document, nullptr);

    const auto element =
        document->getDocumentElement();
    ASSERT_NE(element, nullptr);

    const auto text =
        ::jxx::CAST<::jxx::org::w3c::dom::Text>(
            element->getFirstChild());
    ASSERT_NE(text, nullptr);

    EXPECT_EQ(text->getLength(), 4);
    EXPECT_EQ(
        text->substringData(1, 2)->length(),
        2);
}

TEST(
    XmlDomCharacterDataUtf16Stage,
    SplitUsesUtf16Offsets) {
    const auto document = parseUtf16Data();
    ASSERT_NE(document, nullptr);

    const auto element =
        document->getDocumentElement();
    ASSERT_NE(element, nullptr);

    const auto text =
        ::jxx::CAST<::jxx::org::w3c::dom::Text>(
            element->getFirstChild());
    ASSERT_NE(text, nullptr);

    const auto tail = text->splitText(3);
    ASSERT_NE(tail, nullptr);

    EXPECT_EQ(
        text->getData()->utf8(),
        "A\xF0\x9F\x98\x80");
    EXPECT_EQ(tail->getData()->utf8(), "B");
}
