#include <gtest/gtest.h>
#include <type_traits>
#include "ext/xml/bind/annotation/jxx.ext.xml.bind.annotation.XmlEnum.h"
#include "ext/xml/bind/annotation/jxx.ext.xml.bind.annotation.XmlEnumValue.h"
#include "ext/xml/bind/annotation/jxx.ext.xml.bind.annotation.XmlRootElement.h"
#include "ext/xml/bind/annotation/jxx.ext.xml.bind.annotation.XmlType.h"
#include "ext/xml/bind/metadata/jxx.ext.xml.bind.metadata.EnumValueConverter.h"
#include "ext/xml/bind/metadata/jxx.ext.xml.bind.metadata.EnumValueFormatter.h"
#include "ext/xml/bind/metadata/jxx.ext.xml.bind.metadata.EnumValueMap.h"
namespace {
using namespace ::jxx::ext::xml::bind::annotation;
using namespace ::jxx::ext::xml::bind::metadata;
static_assert(std::is_base_of_v<::jxx::lang::Object, XmlRootElement>);
static_assert(std::is_base_of_v<::jxx::lang::Object, XmlType>);
static_assert(std::is_base_of_v<::jxx::lang::Object, XmlEnum>);
static_assert(std::is_base_of_v<::jxx::lang::Object, XmlEnumValue>);
static_assert(std::is_abstract_v<EnumValueMap>);
static_assert(std::is_base_of_v<ValueConverter, EnumValueConverter>);
static_assert(std::is_base_of_v<ValueFormatter, EnumValueFormatter>);
TEST(JaxbRootTypeEnumTest, PublicRelationshipsAreStable) {
    EXPECT_TRUE((std::is_abstract_v<EnumValueMap>));
    EXPECT_TRUE((std::is_base_of_v<ValueConverter, EnumValueConverter>));
    EXPECT_TRUE((std::is_base_of_v<ValueFormatter, EnumValueFormatter>));
}
} // namespace
