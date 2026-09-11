#include <gtest/gtest.h>

#include "lang/jxx.lang.ArrayIndexOutOfBoundsException.h"
#include "lang/jxx.lang.String.h"
#include "org/xml/sax/helpers/jxx.org.xml.sax.helpers.AttributesImpl.h"

TEST(XmlSaxAttributesImplStage, AddsLooksUpAndMutatesAttributes) {
    const auto attributes = ::jxx::NEW<::jxx::org::xml::sax::helpers::AttributesImpl>();
    const auto uri = ::jxx::NEW<::jxx::lang::String>("urn:test");
    const auto local = ::jxx::NEW<::jxx::lang::String>("id");
    const auto qName = ::jxx::NEW<::jxx::lang::String>("p:id");
    attributes->addAttribute(uri, local, qName,
        ::jxx::NEW<::jxx::lang::String>("CDATA"),
        ::jxx::NEW<::jxx::lang::String>("42"));

    EXPECT_EQ(attributes->getLength(), 1);
    EXPECT_EQ(attributes->getIndex(uri, local), 0);
    EXPECT_EQ(attributes->getIndex(qName), 0);
    EXPECT_EQ(attributes->getValue(qName)->utf8(), "42");
    attributes->setValue(0, ::jxx::NEW<::jxx::lang::String>("43"));
    EXPECT_EQ(attributes->getValue(0)->utf8(), "43");
}

TEST(XmlSaxAttributesImplStage, SnapshotAndBoundsBehavior) {
    const auto source = ::jxx::NEW<::jxx::org::xml::sax::helpers::AttributesImpl>();
    source->addAttribute(nullptr, nullptr,
        ::jxx::NEW<::jxx::lang::String>("name"),
        ::jxx::NEW<::jxx::lang::String>("CDATA"),
        ::jxx::NEW<::jxx::lang::String>("value"));
    const auto snapshot = ::jxx::NEW<::jxx::org::xml::sax::helpers::AttributesImpl>(source);
    source->clear();
    EXPECT_EQ(snapshot->getLength(), 1);
    EXPECT_EQ(snapshot->getURI(9), nullptr);
    EXPECT_THROW(snapshot->removeAttribute(9), ::jxx::lang::ArrayIndexOutOfBoundsException);
}
