#include <gtest/gtest.h>

#include "lang/jxx.lang.String.h"
#include "org/xml/sax/helpers/jxx.org.xml.sax.helpers.DefaultHandler.h"
#include "org/xml/sax/jxx.org.xml.sax.SAXParseException.h"

TEST(XmlSaxHelperStage, DefaultHandlerIgnoresOrdinaryCallbacks) {
    const auto handler =
        ::jxx::NEW<::jxx::org::xml::sax::helpers::DefaultHandler>();
    EXPECT_NO_THROW(handler->startDocument());
    EXPECT_NO_THROW(handler->endDocument());
    EXPECT_EQ(handler->resolveEntity(nullptr, nullptr), nullptr);
}

TEST(XmlSaxHelperStage, DefaultHandlerRethrowsFatalError) {
    const auto handler =
        ::jxx::NEW<::jxx::org::xml::sax::helpers::DefaultHandler>();
    const auto exception =
        ::jxx::NEW<::jxx::org::xml::sax::SAXParseException>(
            ::jxx::NEW<::jxx::lang::String>("fatal"),
            nullptr,
            nullptr,
            1,
            2);
    EXPECT_THROW(
        handler->fatalError(exception),
        ::jxx::org::xml::sax::SAXParseException);
}
