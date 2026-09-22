#include <gtest/gtest.h>
#include <type_traits>
#include "ext/xml/bind/metadata/jxx.ext.xml.bind.metadata.BindingDescriptor.h"
#include "ext/xml/bind/metadata/jxx.ext.xml.bind.metadata.MarshallingDescriptor.h"
#include "ext/xml/bind/metadata/jxx.ext.xml.bind.metadata.MixedContentReader.h"
#include "ext/xml/bind/metadata/jxx.ext.xml.bind.metadata.MixedContentWriter.h"
namespace {
using ::jxx::ext::xml::bind::metadata::BindingDescriptor;
using ::jxx::ext::xml::bind::metadata::MarshallingDescriptor;
using ::jxx::ext::xml::bind::metadata::MixedContentReader;
using ::jxx::ext::xml::bind::metadata::MixedContentWriter;
static_assert(std::is_abstract_v<MixedContentReader>);
static_assert(std::is_abstract_v<MixedContentWriter>);
static_assert(std::is_base_of_v<::jxx::lang::Object, BindingDescriptor>);
static_assert(std::is_base_of_v<::jxx::lang::Object, MarshallingDescriptor>);
TEST(JaxbMixedOrderTypeTest, PublicRelationshipsAreStable) {
    EXPECT_TRUE((std::is_abstract_v<MixedContentReader>));
    EXPECT_TRUE((std::is_abstract_v<MixedContentWriter>));
    EXPECT_TRUE((std::is_base_of_v<::jxx::lang::Object, BindingDescriptor>));
    EXPECT_TRUE((std::is_base_of_v<::jxx::lang::Object, MarshallingDescriptor>));
}
}
