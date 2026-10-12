#include <gtest/gtest.h>

#include "ext/xml/validation/jxx.ext.xml.validation.ClassInitialization.h"
#include "ext/xml/validation/jxx.ext.xml.validation.Schema.h"
#include "jxx.initialize.h"

TEST(JxxXmlValidationClassInitialization, SchemaHasClassMetadata)
{
    ::jxx::initialize();

    const auto schemaClass =
        ::jxx::ext::xml::validation::Schema::Class();

    ASSERT_NE(schemaClass, nullptr);
}

TEST(JxxXmlValidationClassInitialization, RepeatedInitializationIsIdempotent)
{
    const auto before =
        ::jxx::ext::xml::validation::Schema::Class();

    EXPECT_NO_THROW(
        ::jxx::ext::xml::validation::initializeClasses());
    EXPECT_NO_THROW(
        ::jxx::ext::xml::validation::initializeClasses());

    EXPECT_EQ(
        before,
        ::jxx::ext::xml::validation::Schema::Class());
}
