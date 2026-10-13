#include <gtest/gtest.h>

#include "ext/xml/bind/jxx.ext.xml.bind.ClassInitialization.h"
#include "ext/xml/bind/jxx.ext.xml.bind.JAXBException.h"
#include "ext/xml/bind/jxx.ext.xml.bind.UnmarshalException.h"
#include "jxx.initialize.h"

TEST(JxxXmlBindClassInitialization, MetadataTypesAreInitialized)
{
    ::jxx::initialize();

    const auto jaxbExceptionClass =
        ::jxx::ext::xml::bind::JAXBException::JxxClassInfoMarker::Class();
    const auto unmarshalExceptionClass =
        ::jxx::ext::xml::bind::UnmarshalException::JxxClassInfoMarker::Class();

    ASSERT_NE(jaxbExceptionClass, nullptr);
    ASSERT_NE(unmarshalExceptionClass, nullptr);
    EXPECT_NE(jaxbExceptionClass, unmarshalExceptionClass);
}

TEST(JxxXmlBindClassInitialization, RepeatedInitializationIsIdempotent)
{
    const auto jaxbExceptionBefore =
        ::jxx::ext::xml::bind::JAXBException::JxxClassInfoMarker::Class();
    const auto unmarshalExceptionBefore =
        ::jxx::ext::xml::bind::UnmarshalException::JxxClassInfoMarker::Class();

    EXPECT_NO_THROW(::jxx::ext::xml::bind::initializeClasses());
    EXPECT_NO_THROW(::jxx::ext::xml::bind::initializeClasses());

    EXPECT_EQ(
        jaxbExceptionBefore,
        ::jxx::ext::xml::bind::JAXBException::JxxClassInfoMarker::Class());
    EXPECT_EQ(
        unmarshalExceptionBefore,
        ::jxx::ext::xml::bind::UnmarshalException::JxxClassInfoMarker::Class());
}
