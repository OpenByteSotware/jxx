#include <gtest/gtest.h>

#include "ext/xml/xpath/internal/jxx.ext.xml.xpath.internal.ClassInitialization.h"
#include "ext/xml/xpath/internal/jxx.ext.xml.xpath.internal.DomXPath.h"
#include "jxx.initialize.h"

namespace {

auto domXPathClass()
{
    return ::jxx::lang::ClassBase<
        ::jxx::ext::xml::xpath::internal::DomXPath,
        ::jxx::lang::Object,
        ::jxx::ext::xml::xpath::XPath>::Class();
}

} // namespace

TEST(JxxXmlXPathInternalClassInitialization, DomXPathHasClassMetadata)
{
    ::jxx::initialize();

    EXPECT_NE(domXPathClass(), nullptr);
}

TEST(JxxXmlXPathInternalClassInitialization, RepeatedInitializationIsIdempotent)
{
    const auto before = domXPathClass();

    EXPECT_NO_THROW(::jxx::ext::xml::xpath::internal::initializeClasses());
    EXPECT_NO_THROW(::jxx::ext::xml::xpath::internal::initializeClasses());

    EXPECT_EQ(before, domXPathClass());
}
