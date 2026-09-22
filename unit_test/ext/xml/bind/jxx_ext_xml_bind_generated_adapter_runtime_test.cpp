#include <gtest/gtest.h>

#include "ext/xml/bind/metadata/jxx.ext.xml.bind.metadata.PropertyBinding.h"
#include "lang/jxx.lang.String.h"
#include "generated_binding_adapters.h"

TEST(JaxbGeneratedAdapterRuntimeTest, ConvertsAndWritesElementValue) {
    const auto binding =
        ::jxx::test::generated::portXmlBinding();
    const auto target =
        ::jxx::test::generated::newPortModel(0);
    const auto value = binding->converter()->convert(
        ::jxx::NEW<::jxx::lang::String>("1373"));

    binding->writer()->write(target, value);

    EXPECT_EQ(
        1373,
        ::jxx::test::generated::portFromModel(target));
    EXPECT_TRUE(binding->required());
    EXPECT_TRUE(binding->isTextValue());
}
