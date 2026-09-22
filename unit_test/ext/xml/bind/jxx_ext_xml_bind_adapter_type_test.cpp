#include <gtest/gtest.h>
#include <type_traits>

#include "ext/xml/bind/annotation/adapters/jxx.ext.xml.bind.annotation.adapters.XmlAdapter.h"
#include "ext/xml/bind/annotation/adapters/jxx.ext.xml.bind.annotation.adapters.XmlJavaTypeAdapter.h"
#include "ext/xml/bind/annotation/adapters/jxx.ext.xml.bind.annotation.adapters.XmlJavaTypeAdapters.h"
#include "ext/xml/bind/metadata/jxx.ext.xml.bind.metadata.AdaptedValueConverter.h"
#include "ext/xml/bind/metadata/jxx.ext.xml.bind.metadata.AdaptedValueFormatter.h"

namespace {
using namespace ::jxx::ext::xml::bind::annotation::adapters;
using ::jxx::ext::xml::bind::metadata::AdaptedValueConverter;
using ::jxx::ext::xml::bind::metadata::AdaptedValueFormatter;
using ::jxx::ext::xml::bind::metadata::ValueConverter;
using ::jxx::ext::xml::bind::metadata::ValueFormatter;

static_assert(std::is_abstract_v<XmlAdapter>);
static_assert(std::is_base_of_v<::jxx::lang::Object, XmlJavaTypeAdapter>);
static_assert(std::is_base_of_v<::jxx::lang::Object, XmlJavaTypeAdapters>);
static_assert(std::is_base_of_v<ValueConverter, AdaptedValueConverter>);
static_assert(std::is_base_of_v<ValueFormatter, AdaptedValueFormatter>);

TEST(JaxbAdapterTypeTest, PublicRelationshipsAreStable) {
    EXPECT_TRUE((std::is_abstract_v<XmlAdapter>));
    EXPECT_TRUE((std::is_base_of_v<ValueConverter, AdaptedValueConverter>));
    EXPECT_TRUE((std::is_base_of_v<ValueFormatter, AdaptedValueFormatter>));
}
} // namespace
