#include <gtest/gtest.h>
#include <type_traits>
#include "ext/xml/bind/metadata/jxx.ext.xml.bind.metadata.MarshallingBinding.h"
#include "ext/xml/bind/metadata/jxx.ext.xml.bind.metadata.PropertyBinding.h"
namespace {
using ::jxx::ext::xml::bind::metadata::MarshallingBinding;
using ::jxx::ext::xml::bind::metadata::PropertyBinding;
static_assert(std::is_base_of_v<::jxx::lang::Object, PropertyBinding>);
static_assert(std::is_base_of_v<::jxx::lang::Object, MarshallingBinding>);
TEST(JaxbDefaultFixedTypeTest, MetadataTypesRemainObjects) {
    EXPECT_TRUE((std::is_base_of_v<::jxx::lang::Object, PropertyBinding>));
    EXPECT_TRUE((std::is_base_of_v<::jxx::lang::Object, MarshallingBinding>));
}
} // namespace
