#include <gtest/gtest.h>
#include <type_traits>
#include "ext/xml/bind/metadata/jxx.ext.xml.bind.metadata.BindingDescriptor.h"
#include "ext/xml/bind/metadata/jxx.ext.xml.bind.metadata.MarshallingDescriptor.h"
#include "ext/xml/bind/metadata/jxx.ext.xml.bind.metadata.MixedContentReader.h"
#include "ext/xml/bind/metadata/jxx.ext.xml.bind.metadata.MixedContentWriter.h"
#include "ext/xml/bind/metadata/jxx.ext.xml.bind.metadata.LifecycleCallback.h"
namespace {
using namespace ::jxx::ext::xml::bind::metadata;
static_assert(std::is_abstract_v<MixedContentReader>);
static_assert(std::is_abstract_v<MixedContentWriter>);
static_assert(std::is_abstract_v<LifecycleCallback>);
TEST(JaxbMixedLifecycleTypeTest, ContractsCoexist) {
    EXPECT_TRUE((std::is_abstract_v<MixedContentReader>));
    EXPECT_TRUE((std::is_abstract_v<MixedContentWriter>));
    EXPECT_TRUE((std::is_abstract_v<LifecycleCallback>));
}
}
