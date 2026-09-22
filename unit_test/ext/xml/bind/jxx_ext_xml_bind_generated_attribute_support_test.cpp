#include <gtest/gtest.h>
#include <type_traits>
#include "ext/xml/bind/metadata/jxx.ext.xml.bind.metadata.BuiltinFormatters.h"
#include "ext/xml/bind/metadata/jxx.ext.xml.bind.metadata.LexicalValidator.h"
#include "ext/xml/bind/metadata/jxx.ext.xml.bind.metadata.ValidatingValueConverter.h"
#include "ext/xml/bind/metadata/jxx.ext.xml.bind.metadata.ValidatingValueFormatter.h"
namespace {
using namespace ::jxx::ext::xml::bind::metadata;
static_assert(std::is_base_of_v<ValueFormatter, StringFormatter>);
static_assert(std::is_base_of_v<ValueFormatter, IntegerFormatter>);
static_assert(std::is_base_of_v<ValueFormatter, LongFormatter>);
static_assert(std::is_base_of_v<ValueFormatter, DoubleFormatter>);
static_assert(std::is_base_of_v<ValueFormatter, BooleanFormatter>);
static_assert(std::is_abstract_v<LexicalValidator>);
static_assert(std::is_base_of_v<ValueConverter, ValidatingValueConverter>);
static_assert(std::is_base_of_v<ValueFormatter, ValidatingValueFormatter>);
TEST(JaxbGeneratedAttributeSupportTest, RuntimeContractsAreAvailable) {
    EXPECT_TRUE((std::is_abstract_v<LexicalValidator>));
    EXPECT_TRUE((std::is_base_of_v<ValueConverter, ValidatingValueConverter>));
    EXPECT_TRUE((std::is_base_of_v<ValueFormatter, ValidatingValueFormatter>));
}
} // namespace
