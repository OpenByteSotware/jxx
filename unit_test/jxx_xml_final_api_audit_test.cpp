#include <gtest/gtest.h>

#include "lang/jxx.lang.String.h"
#include "org/xml/sax/helpers/jxx.org.xml.sax.helpers.ParserFactory.h"
#include "org/xml/sax/jxx.org.xml.sax.Parser.h"
#include "lang/jxx.lang.NullPointerException.h"

TEST(XmlFinalApiAuditStage, ParserFactoryCreatesDefaultParser) {
    const auto parser =
        ::jxx::org::xml::sax::helpers::ParserFactory::makeParser();
    EXPECT_NE(parser, nullptr);
}

TEST(XmlFinalApiAuditStage, ParserFactoryCreatesNamedDefaultReader) {
    const auto parser =
        ::jxx::org::xml::sax::helpers::ParserFactory::makeParser(
            ::jxx::NEW<::jxx::lang::String>(
                "org.xml.sax.internal.PugiXMLReader"));
    EXPECT_NE(parser, nullptr);
}

TEST(XmlFinalApiAuditStage, ParserFactoryRejectsNullClassName) {
    EXPECT_THROW(
        ::jxx::org::xml::sax::helpers::ParserFactory::makeParser(nullptr),
        ::jxx::lang::NullPointerException);
}
