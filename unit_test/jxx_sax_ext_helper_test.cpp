#include <gtest/gtest.h>

#include "lang/jxx.lang.String.h"
#include "org/xml/sax/ext/jxx.org.xml.sax.ext.Attributes2Impl.h"
#include "org/xml/sax/ext/jxx.org.xml.sax.ext.Locator2Impl.h"

TEST(XmlSaxExtHelperStage, Attributes2TracksFlags) {
    const auto attributes =
        ::jxx::NEW<::jxx::org::xml::sax::ext::Attributes2Impl>();

    attributes->addAttribute(
        ::jxx::NEW<::jxx::lang::String>("urn:test"),
        ::jxx::NEW<::jxx::lang::String>("id"),
        ::jxx::NEW<::jxx::lang::String>("p:id"),
        ::jxx::NEW<::jxx::lang::String>("ID"),
        ::jxx::NEW<::jxx::lang::String>("7"));

    EXPECT_TRUE(attributes->isDeclared(0));
    EXPECT_TRUE(attributes->isSpecified(0));

    attributes->setSpecified(0, false);
    EXPECT_FALSE(attributes->isSpecified(0));
}

TEST(XmlSaxExtHelperStage, Locator2CopiesExtendedState) {
    const auto source =
        ::jxx::NEW<::jxx::org::xml::sax::ext::Locator2Impl>();

    source->setLineNumber(4);
    source->setXMLVersion(
        ::jxx::NEW<::jxx::lang::String>("1.0"));
    source->setEncoding(
        ::jxx::NEW<::jxx::lang::String>("UTF-8"));

    const auto snapshot =
        ::jxx::NEW<::jxx::org::xml::sax::ext::Locator2Impl>(source);

    EXPECT_EQ(snapshot->getLineNumber(), 4);
    EXPECT_EQ(snapshot->getXMLVersion()->utf8(), "1.0");
    EXPECT_EQ(snapshot->getEncoding()->utf8(), "UTF-8");
}
