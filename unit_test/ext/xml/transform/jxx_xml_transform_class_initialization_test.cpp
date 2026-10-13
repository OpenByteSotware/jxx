#include <gtest/gtest.h>

#include "ext/xml/transform/jxx.ext.xml.transform.ClassInitialization.h"
#include "ext/xml/transform/jxx.ext.xml.transform.OutputKeys.h"
#include "ext/xml/transform/jxx.ext.xml.transform.Result.h"
#include "ext/xml/transform/jxx.ext.xml.transform.Source.h"
#include "ext/xml/transform/jxx.ext.xml.transform.Transformer.h"
#include "ext/xml/transform/jxx.ext.xml.transform.TransformerConfigurationException.h"
#include "ext/xml/transform/jxx.ext.xml.transform.TransformerException.h"
#include "ext/xml/transform/jxx.ext.xml.transform.TransformerFactory.h"
#include "jxx.initialize.h"

namespace {

using namespace ::jxx::ext::xml::transform;

auto outputKeysClass()
{
    return ::jxx::lang::ClassBase<
        OutputKeys,
        ::jxx::lang::Object>::Class();
}

auto resultClass()
{
    return ::jxx::lang::InterfaceBase<Result>::Class();
}

auto sourceClass()
{
    return ::jxx::lang::InterfaceBase<Source>::Class();
}

auto transformerClass()
{
    return ::jxx::lang::ClassBase<
        Transformer,
        ::jxx::lang::Object>::Class();
}

auto transformerExceptionClass()
{
    return ::jxx::lang::ClassBase<
        TransformerException,
        ::jxx::lang::Exception>::Class();
}

auto transformerConfigurationExceptionClass()
{
    return ::jxx::lang::ClassBase<
        TransformerConfigurationException,
        TransformerException>::Class();
}

auto transformerFactoryClass()
{
    return ::jxx::lang::ClassBase<
        TransformerFactory,
        ::jxx::lang::Object>::Class();
}

} // namespace

TEST(JxxXmlTransformClassInitialization, RootPackageTypesHaveClassMetadata)
{
    ::jxx::initialize();

    EXPECT_NE(outputKeysClass(), nullptr);
    EXPECT_NE(resultClass(), nullptr);
    EXPECT_NE(sourceClass(), nullptr);
    EXPECT_NE(transformerClass(), nullptr);
    EXPECT_NE(transformerExceptionClass(), nullptr);
    EXPECT_NE(transformerConfigurationExceptionClass(), nullptr);
    EXPECT_NE(transformerFactoryClass(), nullptr);
}

TEST(JxxXmlTransformClassInitialization, RepeatedInitializationIsIdempotent)
{
    const auto transformerBefore = transformerClass();
    const auto exceptionBefore = transformerExceptionClass();

    EXPECT_NO_THROW(::jxx::ext::xml::transform::initializeClasses());
    EXPECT_NO_THROW(::jxx::ext::xml::transform::initializeClasses());

    EXPECT_EQ(transformerBefore, transformerClass());
    EXPECT_EQ(exceptionBefore, transformerExceptionClass());
}
