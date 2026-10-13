#include <gtest/gtest.h>

#include "ext/xml/namespace/jxx.ext.xml.namespace.ClassInitialization.h"
#include "ext/xml/namespace/jxx.ext.xml.namespace.QName.h"
#include "jxx.initialize.h"

namespace {

auto qNameClass()
{
    return ::jxx::lang::ClassBase<
        ::jxx::ext::xml::namespace_::QName,
        ::jxx::lang::Object>::Class();
}

} // namespace

TEST(JxxXmlNamespaceClassInitialization, QNameHasClassMetadata)
{
    ::jxx::initialize();

    EXPECT_NE(qNameClass(), nullptr);
}

TEST(JxxXmlNamespaceClassInitialization, RepeatedInitializationIsIdempotent)
{
    const auto before = qNameClass();

    EXPECT_NO_THROW(::jxx::ext::xml::namespace_::initializeClasses());
    EXPECT_NO_THROW(::jxx::ext::xml::namespace_::initializeClasses());

    EXPECT_EQ(before, qNameClass());
}
