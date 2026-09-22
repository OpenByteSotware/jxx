#include <gtest/gtest.h>
#include <type_traits>
#include "ext/xml/bind/annotation/jxx.ext.xml.bind.annotation.XmlAttribute.h"
#include "ext/xml/bind/annotation/jxx.ext.xml.bind.annotation.XmlElementRef.h"
#include "ext/xml/bind/annotation/jxx.ext.xml.bind.annotation.XmlElementRefs.h"
#include "ext/xml/bind/annotation/jxx.ext.xml.bind.annotation.XmlElementWrapper.h"
#include "ext/xml/bind/annotation/jxx.ext.xml.bind.annotation.XmlElements.h"
#include "ext/xml/bind/annotation/jxx.ext.xml.bind.annotation.XmlList.h"
#include "ext/xml/bind/annotation/jxx.ext.xml.bind.annotation.XmlTransient.h"
#include "ext/xml/bind/annotation/jxx.ext.xml.bind.annotation.XmlValue.h"
namespace {
using namespace ::jxx::ext::xml::bind::annotation;
static_assert(std::is_base_of_v<::jxx::lang::Object, XmlAttribute>);
static_assert(std::is_base_of_v<::jxx::lang::Object, XmlElementWrapper>);
static_assert(std::is_base_of_v<::jxx::lang::Object, XmlElementRef>);
static_assert(std::is_base_of_v<::jxx::lang::Object, XmlElementRefs>);
static_assert(std::is_base_of_v<::jxx::lang::Object, XmlElements>);
static_assert(std::is_base_of_v<::jxx::lang::Object, XmlList>);
static_assert(std::is_base_of_v<::jxx::lang::Object, XmlTransient>);
static_assert(std::is_base_of_v<::jxx::lang::Object, XmlValue>);
TEST(JaxbSchemaClassAnnotationsTest, PublicTypesAreAvailable) {
    EXPECT_TRUE((std::is_base_of_v<::jxx::lang::Object, XmlAttribute>));
    EXPECT_TRUE((std::is_base_of_v<::jxx::lang::Object, XmlElementWrapper>));
    EXPECT_TRUE((std::is_base_of_v<::jxx::lang::Object, XmlElementRef>));
}
} // namespace
