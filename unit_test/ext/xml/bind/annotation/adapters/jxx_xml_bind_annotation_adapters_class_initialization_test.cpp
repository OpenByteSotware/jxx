#include <gtest/gtest.h>

#include "jxx.initialize.h"
#include "ext/xml/bind/annotation/adapters/jxx.ext.xml.bind.annotation.adapters.ClassInitialization.h"
#include "ext/xml/bind/annotation/adapters/jxx.ext.xml.bind.annotation.adapters.XmlAdapter.h"
#include "ext/xml/bind/annotation/adapters/jxx.ext.xml.bind.annotation.adapters.XmlJavaTypeAdapter.h"
#include "ext/xml/bind/annotation/adapters/jxx.ext.xml.bind.annotation.adapters.XmlJavaTypeAdapters.h"

TEST(JxxStage51ClassInitialization, MetadataAndIdempotence)
{
    ::jxx::initialize();
    EXPECT_NE((::jxx::lang::InterfaceBase<::jxx::ext::xml::bind::annotation::adapters::XmlAdapter>::Class()), nullptr);
    EXPECT_NE((::jxx::lang::ClassBase<::jxx::ext::xml::bind::annotation::adapters::XmlJavaTypeAdapter, ::jxx::lang::Object>::Class()), nullptr);
    EXPECT_NE((::jxx::lang::ClassBase<::jxx::ext::xml::bind::annotation::adapters::XmlJavaTypeAdapters, ::jxx::lang::Object>::Class()), nullptr);
    const auto before = ::jxx::lang::InterfaceBase<::jxx::ext::xml::bind::annotation::adapters::XmlAdapter>::Class();
    EXPECT_NO_THROW(::jxx::ext::xml::bind::annotation::adapters::initializeClasses());
    EXPECT_NO_THROW(::jxx::ext::xml::bind::annotation::adapters::initializeClasses());
    EXPECT_EQ(before, (::jxx::lang::InterfaceBase<::jxx::ext::xml::bind::annotation::adapters::XmlAdapter>::Class()));
}
