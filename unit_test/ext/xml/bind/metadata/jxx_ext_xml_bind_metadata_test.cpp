#include <gtest/gtest.h>

#include <type_traits>

#include "ext/xml/bind/metadata/jxx.ext.xml.bind.metadata.BindingDescriptor.h"
#include "ext/xml/bind/metadata/jxx.ext.xml.bind.metadata.BuiltinConverters.h"
#include "ext/xml/bind/metadata/jxx.ext.xml.bind.metadata.ObjectFactory.h"
#include "ext/xml/bind/metadata/jxx.ext.xml.bind.metadata.PropertyBinding.h"
#include "ext/xml/bind/metadata/jxx.ext.xml.bind.metadata.PropertyWriter.h"
#include "ext/xml/bind/metadata/jxx.ext.xml.bind.metadata.ValueConverter.h"

namespace {

using ::jxx::ext::xml::bind::metadata::BindingDescriptor;
using ::jxx::ext::xml::bind::metadata::BooleanConverter;
using ::jxx::ext::xml::bind::metadata::DoubleConverter;
using ::jxx::ext::xml::bind::metadata::IntegerConverter;
using ::jxx::ext::xml::bind::metadata::LongConverter;
using ::jxx::ext::xml::bind::metadata::ObjectFactory;
using ::jxx::ext::xml::bind::metadata::PropertyBinding;
using ::jxx::ext::xml::bind::metadata::PropertyWriter;
using ::jxx::ext::xml::bind::metadata::StringConverter;
using ::jxx::ext::xml::bind::metadata::ValueConverter;

static_assert(std::is_abstract_v<ObjectFactory>);
static_assert(std::is_abstract_v<PropertyWriter>);
static_assert(std::is_abstract_v<ValueConverter>);

static_assert(std::is_base_of_v<::jxx::lang::Object, BindingDescriptor>);
static_assert(std::is_base_of_v<::jxx::lang::Object, PropertyBinding>);
static_assert(std::is_base_of_v<ValueConverter, StringConverter>);
static_assert(std::is_base_of_v<ValueConverter, IntegerConverter>);
static_assert(std::is_base_of_v<ValueConverter, LongConverter>);
static_assert(std::is_base_of_v<ValueConverter, DoubleConverter>);
static_assert(std::is_base_of_v<ValueConverter, BooleanConverter>);

TEST(
    JaxbBindingMetadataTest,
    PublicTypesHaveExpectedInterfaceAndObjectRelationships) {

    EXPECT_TRUE((std::is_abstract_v<ObjectFactory>));
    EXPECT_TRUE((std::is_abstract_v<PropertyWriter>));
    EXPECT_TRUE((std::is_abstract_v<ValueConverter>));
    EXPECT_TRUE((std::is_base_of_v<
        ::jxx::lang::Object,
        BindingDescriptor>));
    EXPECT_TRUE((std::is_base_of_v<
        ::jxx::lang::Object,
        PropertyBinding>));
}

TEST(
    JaxbBindingMetadataTest,
    BuiltinConvertersImplementValueConverter) {

    EXPECT_TRUE((std::is_base_of_v<
        ValueConverter,
        StringConverter>));
    EXPECT_TRUE((std::is_base_of_v<
        ValueConverter,
        IntegerConverter>));
    EXPECT_TRUE((std::is_base_of_v<
        ValueConverter,
        LongConverter>));
    EXPECT_TRUE((std::is_base_of_v<
        ValueConverter,
        DoubleConverter>));
    EXPECT_TRUE((std::is_base_of_v<
        ValueConverter,
        BooleanConverter>));
}

} // namespace
