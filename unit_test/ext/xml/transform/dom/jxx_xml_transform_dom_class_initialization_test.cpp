#include <gtest/gtest.h>

#include "ext/xml/transform/dom/jxx.ext.xml.transform.dom.ClassInitialization.h"
#include "ext/xml/transform/dom/jxx.ext.xml.transform.dom.DOMSource.h"
#include "jxx.initialize.h"

namespace {

auto domSourceClass()
{
    return ::jxx::lang::ClassBase<
        ::jxx::ext::xml::transform::dom::DOMSource,
        ::jxx::lang::Object,
        ::jxx::ext::xml::transform::Source>::Class();
}

} // namespace

TEST(JxxXmlTransformDomClassInitialization, DOMSourceHasClassMetadata)
{
    ::jxx::initialize();

    EXPECT_NE(domSourceClass(), nullptr);
}

TEST(JxxXmlTransformDomClassInitialization, RepeatedInitializationIsIdempotent)
{
    const auto before = domSourceClass();

    EXPECT_NO_THROW(
        ::jxx::ext::xml::transform::dom::initializeClasses());
    EXPECT_NO_THROW(
        ::jxx::ext::xml::transform::dom::initializeClasses());

    EXPECT_EQ(before, domSourceClass());
}
