#include <gtest/gtest.h>

#include "lang/jxx.lang.String.h"
#include "org/xml/sax/helpers/jxx.org.xml.sax.helpers.AttributeListImpl.h"
#include "org/xml/sax/jxx.org.xml.sax.HandlerBase.h"
#include "org/xml/sax/jxx.org.xml.sax.SAXParseException.h"

TEST(XmlSaxLegacyHelpersStage, AttributeListImplSupportsSnapshotAndLookup) {
    const auto source =
        ::jxx::NEW<::jxx::org::xml::sax::helpers::AttributeListImpl>();

    source->addAttribute(
        ::jxx::NEW<::jxx::lang::String>("id"),
        ::jxx::NEW<::jxx::lang::String>("ID"),
        ::jxx::NEW<::jxx::lang::String>("7"));

    const auto snapshot =
        ::jxx::NEW<::jxx::org::xml::sax::helpers::AttributeListImpl>(source);

    source->clear();

    EXPECT_EQ(snapshot->getLength(), 1);
    EXPECT_EQ(
        snapshot->getValue(
            ::jxx::NEW<::jxx::lang::String>("id"))->utf8(),
        "7");
}

TEST(XmlSaxLegacyHelpersStage, HandlerBaseRethrowsFatalErrors) {
    const auto handler =
        ::jxx::NEW<::jxx::org::xml::sax::HandlerBase>();

    const auto exception =
        ::jxx::NEW<::jxx::org::xml::sax::SAXParseException>(
            ::jxx::NEW<::jxx::lang::String>("fatal"),
            nullptr,
            nullptr,
            1,
            1);

    EXPECT_THROW(
        handler->fatalError(exception),
        ::jxx::org::xml::sax::SAXParseException);
}
