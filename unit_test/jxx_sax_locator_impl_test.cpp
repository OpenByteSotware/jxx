#include <gtest/gtest.h>

#include "lang/jxx.lang.String.h"
#include "org/xml/sax/helpers/jxx.org.xml.sax.helpers.LocatorImpl.h"

TEST(XmlSaxLocatorImplStage, StoresLocationValues) {
    const auto locator =
        ::jxx::NEW<::jxx::org::xml::sax::helpers::LocatorImpl>();
    locator->setPublicId(::jxx::NEW<::jxx::lang::String>("public"));
    locator->setSystemId(::jxx::NEW<::jxx::lang::String>("system"));
    locator->setLineNumber(12);
    locator->setColumnNumber(34);

    EXPECT_EQ(locator->getPublicId()->utf8(), "public");
    EXPECT_EQ(locator->getSystemId()->utf8(), "system");
    EXPECT_EQ(locator->getLineNumber(), 12);
    EXPECT_EQ(locator->getColumnNumber(), 34);
}

TEST(XmlSaxLocatorImplStage, CopyConstructorCreatesSnapshot) {
    const auto source =
        ::jxx::NEW<::jxx::org::xml::sax::helpers::LocatorImpl>();
    source->setLineNumber(7);
    source->setColumnNumber(9);

    const auto snapshot =
        ::jxx::NEW<::jxx::org::xml::sax::helpers::LocatorImpl>(source);
    source->setLineNumber(100);

    EXPECT_EQ(snapshot->getLineNumber(), 7);
    EXPECT_EQ(snapshot->getColumnNumber(), 9);
}
