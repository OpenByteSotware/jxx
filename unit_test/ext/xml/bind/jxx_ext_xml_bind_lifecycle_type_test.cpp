#include <gtest/gtest.h>
#include <type_traits>
#include "ext/xml/bind/metadata/jxx.ext.xml.bind.metadata.LifecycleCallback.h"
#include "ext/xml/bind/metadata/jxx.ext.xml.bind.metadata.BindingDescriptor.h"
#include "ext/xml/bind/metadata/jxx.ext.xml.bind.metadata.MarshallingDescriptor.h"
namespace {
using ::jxx::ext::xml::bind::metadata::LifecycleCallback;
using ::jxx::ext::xml::bind::metadata::BindingDescriptor;
using ::jxx::ext::xml::bind::metadata::MarshallingDescriptor;
static_assert(std::is_abstract_v<LifecycleCallback>);
static_assert(std::is_base_of_v<::jxx::lang::Object, BindingDescriptor>);
static_assert(std::is_base_of_v<::jxx::lang::Object, MarshallingDescriptor>);
TEST(JaxbLifecycleTypeTest, PublicRelationshipsAreStable) {
    EXPECT_TRUE((std::is_abstract_v<LifecycleCallback>));
    EXPECT_TRUE((std::is_base_of_v<::jxx::lang::Object, BindingDescriptor>));
    EXPECT_TRUE((std::is_base_of_v<::jxx::lang::Object, MarshallingDescriptor>));
}
}
