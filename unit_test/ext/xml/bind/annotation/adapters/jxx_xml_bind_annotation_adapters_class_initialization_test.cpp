#include <gtest/gtest.h>

#include "jxx.initialize.h"
#include "ext/xml/bind/annotation/adapters/jxx.ext.xml.bind.annotation.adapters.ClassInitialization.h"
#include "ext/xml/bind/annotation/adapters/jxx.ext.xml.bind.annotation.adapters.XmlAdapter.h"
#include "ext/xml/bind/annotation/adapters/jxx.ext.xml.bind.annotation.adapters.XmlJavaTypeAdapter.h"
#include "ext/xml/bind/annotation/adapters/jxx.ext.xml.bind.annotation.adapters.XmlJavaTypeAdapters.h"

TEST(JxxStage51ClassInitialization, MetadataAndIdempotence)
{
    ::jxx::initialize();
    EXPECT_NE((::jxx::lang::InterfaceBase<XmlAdapter>::Class()), nullptr);
    EXPECT_NE((::jxx::lang::ClassBase<XmlJavaTypeAdapter, ::jxx::lang::Object>::Class()), nullptr);
    EXPECT_NE((::jxx::lang::ClassBase<XmlJavaTypeAdapters, ::jxx::lang::Object>::Class()), nullptr);
    const auto before = ::jxx::lang::InterfaceBase<XmlAdapter>::Class();
    EXPECT_NO_THROW(::jxx::ext::xml::bind::annotation::adapters::initializeClasses());
    EXPECT_NO_THROW(::jxx::ext::xml::bind::annotation::adapters::initializeClasses());
    EXPECT_EQ(before, (::jxx::lang::InterfaceBase<XmlAdapter>::Class()));
}
