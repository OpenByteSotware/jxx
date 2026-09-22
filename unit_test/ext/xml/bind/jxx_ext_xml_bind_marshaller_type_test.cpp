#include <gtest/gtest.h>
#include <type_traits>
#include "ext/xml/bind/jxx.ext.xml.bind.Marshaller.h"
#include "ext/xml/bind/internal/jxx.ext.xml.bind.internal.XmlMarshaller.h"
#include "ext/xml/bind/metadata/jxx.ext.xml.bind.metadata.MarshallingDescriptor.h"
namespace {
using ::jxx::ext::xml::bind::Marshaller;
using ::jxx::ext::xml::bind::internal::XmlMarshaller;
using ::jxx::ext::xml::bind::metadata::MarshallingDescriptor;
static_assert(std::is_abstract_v<Marshaller>);
static_assert(std::is_base_of_v<Marshaller, XmlMarshaller>);
static_assert(std::is_base_of_v<::jxx::lang::Object, MarshallingDescriptor>);
TEST(JaxbMarshallerTypeTest, PublicRelationshipsAreStable) {
    EXPECT_TRUE((std::is_abstract_v<Marshaller>));
    EXPECT_TRUE((std::is_base_of_v<Marshaller, XmlMarshaller>));
    EXPECT_TRUE((std::is_base_of_v<::jxx::lang::Object, MarshallingDescriptor>));
}
}
