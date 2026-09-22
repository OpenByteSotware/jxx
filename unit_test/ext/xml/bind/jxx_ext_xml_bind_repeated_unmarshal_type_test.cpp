#include <gtest/gtest.h>
#include <type_traits>
#include "ext/xml/bind/metadata/jxx.ext.xml.bind.metadata.PropertyBinding.h"
#include "ext/xml/bind/metadata/jxx.ext.xml.bind.metadata.RepeatedPropertyWriter.h"
namespace {
using ::jxx::ext::xml::bind::metadata::PropertyBinding;
using ::jxx::ext::xml::bind::metadata::RepeatedPropertyWriter;
static_assert(std::is_abstract_v<RepeatedPropertyWriter>);
static_assert(std::is_base_of_v<::jxx::lang::Object, PropertyBinding>);
TEST(JaxbRepeatedUnmarshalTypeTest, PublicRelationshipsAreStable) {
    EXPECT_TRUE((std::is_abstract_v<RepeatedPropertyWriter>));
    EXPECT_TRUE((std::is_base_of_v<::jxx::lang::Object, PropertyBinding>));
}
}
