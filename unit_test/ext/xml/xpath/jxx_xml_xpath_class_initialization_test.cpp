#include <gtest/gtest.h>

#include "ext/xml/xpath/jxx.ext.xml.xpath.ClassInitialization.h"
#include "ext/xml/xpath/jxx.ext.xml.xpath.XPath.h"
#include "ext/xml/xpath/jxx.ext.xml.xpath.XPathConstants.h"
#include "ext/xml/xpath/jxx.ext.xml.xpath.XPathExpression.h"
#include "ext/xml/xpath/jxx.ext.xml.xpath.XPathExpressionException.h"
#include "ext/xml/xpath/jxx.ext.xml.xpath.XPathFactory.h"
#include "jxx.initialize.h"

namespace {

using namespace ::jxx::ext::xml::xpath;

auto xpathClass()
{
    return ::jxx::lang::InterfaceBase<XPath>::Class();
}

auto xpathConstantsClass()
{
    return ::jxx::lang::ClassBase<
        XPathConstants,
        ::jxx::lang::Object>::Class();
}

auto xpathExpressionClass()
{
    return ::jxx::lang::InterfaceBase<XPathExpression>::Class();
}

auto xpathExpressionExceptionClass()
{
    return ::jxx::lang::ClassBase<
        XPathExpressionException,
        ::jxx::lang::Exception>::Class();
}

auto xpathFactoryClass()
{
    return ::jxx::lang::ClassBase<
        XPathFactory,
        ::jxx::lang::Object>::Class();
}

} // namespace

TEST(JxxXmlXPathClassInitialization, RootPackageTypesHaveClassMetadata)
{
    ::jxx::initialize();

    EXPECT_NE(xpathClass(), nullptr);
    EXPECT_NE(xpathConstantsClass(), nullptr);
    EXPECT_NE(xpathExpressionClass(), nullptr);
    EXPECT_NE(xpathExpressionExceptionClass(), nullptr);
    EXPECT_NE(xpathFactoryClass(), nullptr);
}

TEST(JxxXmlXPathClassInitialization, RepeatedInitializationIsIdempotent)
{
    const auto xpathBefore = xpathClass();
    const auto factoryBefore = xpathFactoryClass();

    EXPECT_NO_THROW(::jxx::ext::xml::xpath::initializeClasses());
    EXPECT_NO_THROW(::jxx::ext::xml::xpath::initializeClasses());

    EXPECT_EQ(xpathBefore, xpathClass());
    EXPECT_EQ(factoryBefore, xpathFactoryClass());
}
