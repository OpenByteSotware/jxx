#include <gtest/gtest.h>

#include "ext/xml/transform/internal/jxx.ext.xml.transform.internal.ClassInitialization.h"
#include "ext/xml/transform/internal/jxx.ext.xml.transform.internal.IdentityTransformer.h"
#include "ext/xml/transform/internal/jxx.ext.xml.transform.internal.IdentityTransformerFactory.h"
#include "jxx.initialize.h"

namespace {

auto identityTransformerClass()
{
    return ::jxx::lang::ClassBase<
        ::jxx::ext::xml::transform::internal::IdentityTransformer,
        ::jxx::ext::xml::transform::Transformer>::Class();
}

auto identityTransformerFactoryClass()
{
    return ::jxx::lang::ClassBase<
        ::jxx::ext::xml::transform::internal::IdentityTransformerFactory,
        ::jxx::ext::xml::transform::TransformerFactory>::Class();
}

} // namespace

TEST(JxxXmlTransformInternalClassInitialization, InternalTypesHaveClassMetadata)
{
    ::jxx::initialize();

    EXPECT_NE(identityTransformerClass(), nullptr);
    EXPECT_NE(identityTransformerFactoryClass(), nullptr);
    EXPECT_NE(identityTransformerClass(), identityTransformerFactoryClass());
}

TEST(JxxXmlTransformInternalClassInitialization, RepeatedInitializationIsIdempotent)
{
    const auto transformerBefore = identityTransformerClass();
    const auto factoryBefore = identityTransformerFactoryClass();

    EXPECT_NO_THROW(
        ::jxx::ext::xml::transform::internal::initializeClasses());
    EXPECT_NO_THROW(
        ::jxx::ext::xml::transform::internal::initializeClasses());

    EXPECT_EQ(transformerBefore, identityTransformerClass());
    EXPECT_EQ(factoryBefore, identityTransformerFactoryClass());
}
