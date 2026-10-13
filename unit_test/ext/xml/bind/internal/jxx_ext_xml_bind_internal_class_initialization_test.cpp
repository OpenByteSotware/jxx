#include <gtest/gtest.h>

#include "jxx.initialize.h"
#include "ext/xml/bind/internal/jxx.ext.xml.bind.internal.ClassInitialization.h"
#include "ext/xml/bind/internal/jxx.ext.xml.bind.internal.DomUnmarshaller.h"
#include "ext/xml/bind/internal/jxx.ext.xml.bind.internal.XmlMarshaller.h"

TEST(JxxStage49ClassInitialization, MetadataAndIdempotence)
{
    ::jxx::initialize();
    EXPECT_NE((::jxx::lang::ClassBase<::jxx::ext::xml::bind::internal::DomUnmarshaller, ::jxx::lang::Object, ::jxx::ext::xml::bind::Unmarshaller>::Class()), nullptr);
    EXPECT_NE((::jxx::lang::ClassBase<::jxx::ext::xml::bind::internal::XmlMarshaller, ::jxx::lang::Object, ::jxx::ext::xml::bind::Marshaller>::Class()), nullptr);
    const auto before = ::jxx::lang::ClassBase<::jxx::ext::xml::bind::internal::DomUnmarshaller, ::jxx::lang::Object, ::jxx::ext::xml::bind::Unmarshaller>::Class();
    EXPECT_NO_THROW(::jxx::ext::xml::bind::internal::initializeClasses());
    EXPECT_NO_THROW(::jxx::ext::xml::bind::internal::initializeClasses());
    EXPECT_EQ(before, (::jxx::lang::ClassBase<::jxx::ext::xml::bind::internal::DomUnmarshaller, ::jxx::lang::Object, ::jxx::ext::xml::bind::Unmarshaller>::Class()));
}
