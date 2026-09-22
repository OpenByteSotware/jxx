#include <gtest/gtest.h>
#include <type_traits>
#include "ext/xml/bind/metadata/jxx.ext.xml.bind.metadata.MarshallingBinding.h"
#include "ext/xml/bind/metadata/jxx.ext.xml.bind.metadata.RepeatedValueReader.h"
namespace {
using ::jxx::ext::xml::bind::metadata::MarshallingBinding;
using ::jxx::ext::xml::bind::metadata::RepeatedValueReader;
static_assert(std::is_abstract_v<RepeatedValueReader>);
static_assert(std::is_base_of_v<::jxx::lang::Object, MarshallingBinding>);
TEST(JaxbRepeatedNamespaceTypeTest, PublicRelationshipsAreStable) {
    EXPECT_TRUE((std::is_abstract_v<RepeatedValueReader>));
    EXPECT_TRUE((std::is_base_of_v<::jxx::lang::Object, MarshallingBinding>));
}
}
