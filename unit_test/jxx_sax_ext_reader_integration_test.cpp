#include <gtest/gtest.h>

#include "io/jxx.io.StringReader.h"
#include "lang/jxx.lang.String.h"
#include "org/xml/sax/ext/jxx.org.xml.sax.ext.Attributes2.h"
#include "org/xml/sax/ext/jxx.org.xml.sax.ext.DefaultHandler2.h"
#include "org/xml/sax/ext/jxx.org.xml.sax.ext.Locator2.h"
#include "org/xml/sax/helpers/jxx.org.xml.sax.helpers.XMLReaderFactory.h"
#include "org/xml/sax/jxx.org.xml.sax.InputSource.h"
#include "org/xml/sax/jxx.org.xml.sax.XMLReader.h"

namespace {

class ExtensionHandler final
    : public ::jxx::org::xml::sax::ext::DefaultHandler2 {
public:
    bool sawLocator2 = false;
    bool sawAttributes2 = false;
    bool sawComment = false;
    bool sawCdataStart = false;
    bool sawCdataEnd = false;

    void setDocumentLocator(
        const ::jxx::Ptr<::jxx::org::xml::sax::Locator>& locator) override {
        sawLocator2 =
            ::jxx::CAST<::jxx::org::xml::sax::ext::Locator2>(locator) != nullptr;
    }

    void startElement(
        const ::jxx::Ptr<::jxx::lang::String>& uri,
        const ::jxx::Ptr<::jxx::lang::String>& localName,
        const ::jxx::Ptr<::jxx::lang::String>& qualifiedName,
        const ::jxx::Ptr<::jxx::org::xml::sax::Attributes>& attributes) override {
        (void)uri;
        (void)localName;
        (void)qualifiedName;
        sawAttributes2 =
            ::jxx::CAST<::jxx::org::xml::sax::ext::Attributes2>(attributes) != nullptr;
    }

    void comment(
        const ::jxx::lang::CharArray& value,
        ::jxx::lang::jint start,
        ::jxx::lang::jint length) override {
        (void)value;
        (void)start;
        (void)length;
        sawComment = true;
    }

    void startCDATA() override { sawCdataStart = true; }
    void endCDATA() override { sawCdataEnd = true; }
};

} // namespace

TEST(XmlSaxExtensionIntegrationStage, ReportsExtendedObjectsAndLexicalEvents) {
    const auto reader =
        ::jxx::org::xml::sax::helpers::XMLReaderFactory::createXMLReader();
    const auto handler = ::jxx::NEW<ExtensionHandler>();

    reader->setContentHandler(handler);
    reader->setProperty(
        ::jxx::NEW<::jxx::lang::String>(
            "http://xml.org/sax/properties/lexical-handler"),
        ::jxx::CAST<::jxx::lang::Object>(handler));

    const auto input =
        ::jxx::NEW<::jxx::org::xml::sax::InputSource>(
            ::jxx::NEW<::jxx::io::StringReader>(
                ::jxx::NEW<::jxx::lang::String>(
                    "<root id='7'><!--note--><![CDATA[value]]></root>")));

    reader->parse(input);

    EXPECT_TRUE(handler->sawLocator2);
    EXPECT_TRUE(handler->sawAttributes2);
    EXPECT_TRUE(handler->sawComment);
    EXPECT_TRUE(handler->sawCdataStart);
    EXPECT_TRUE(handler->sawCdataEnd);
}
