#include <gtest/gtest.h>
#include <type_traits>
#include "ext/xml/bind/metadata/jxx.ext.xml.bind.metadata.BindingDescriptor.h"
#include "ext/xml/bind/metadata/jxx.ext.xml.bind.metadata.PropertyBinding.h"
#include "ext/xml/bind/metadata/jxx.ext.xml.bind.metadata.PropertyWriter.h"
#include "ext/xml/bind/metadata/jxx.ext.xml.bind.metadata.ValueConverter.h"
namespace {
using ::jxx::ext::xml::bind::metadata::BindingDescriptor;
using ::jxx::ext::xml::bind::metadata::PropertyBinding;
using ::jxx::ext::xml::bind::metadata::PropertyWriter;
using ::jxx::ext::xml::bind::metadata::ValueConverter;
static_assert(std::is_base_of_v<::jxx::lang::Object, BindingDescriptor>);
static_assert(std::is_base_of_v<::jxx::lang::Object, PropertyBinding>);
static_assert(std::is_abstract_v<PropertyWriter>);
static_assert(std::is_abstract_v<ValueConverter>);
TEST(JaxbAdapterBehaviorTest, BindingContractsAreAvailable) {
    EXPECT_TRUE((std::is_base_of_v<::jxx::lang::Object, BindingDescriptor>));
    EXPECT_TRUE((std::is_base_of_v<::jxx::lang::Object, PropertyBinding>));
    EXPECT_TRUE((std::is_abstract_v<PropertyWriter>));
    EXPECT_TRUE((std::is_abstract_v<ValueConverter>));
}
} // namespace
