#include <gtest/gtest.h>

#include "ext/xml/parsers/jxx.ext.xml.parsers.ParserConfigurationException.h"
#include "jxx.initialize.h"
#include "lang/jxx.lang.String.h"
#include "org/xml/sax/jxx.org.xml.sax.SAXParseException.h"

TEST(XmlExceptionStage, ParserConfigurationExceptionCopies) {
    auto message = ::jxx::NEW<::jxx::lang::String>("configuration");
    ::jxx::ext::xml::parsers::ParserConfigurationException original(message);
    auto copy = original;
    EXPECT_EQ(copy.getMessage()->utf8(), "configuration");
}

TEST(XmlExceptionStage, SaxParseLocationIsPreserved) {
    ::jxx::org::xml::sax::SAXParseException exception(
        ::jxx::NEW<::jxx::lang::String>("bad xml"),
        nullptr,
        ::jxx::NEW<::jxx::lang::String>("input.xml"),
        7,
        12);
    EXPECT_EQ(exception.getLineNumber(), 7);
    EXPECT_EQ(exception.getColumnNumber(), 12);
    EXPECT_EQ(exception.getSystemId()->utf8(), "input.xml");
}

TEST(XmlExceptionStage, PackageInitializationIsIdempotent) {
    EXPECT_NO_THROW(::jxx::initialize());
    EXPECT_NO_THROW(::jxx::initialize());
}
