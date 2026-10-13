#include <gtest/gtest.h>

#include "ext/xml/transform/stream/jxx.ext.xml.transform.stream.ClassInitialization.h"
#include "ext/xml/transform/stream/jxx.ext.xml.transform.stream.StreamResult.h"
#include "jxx.initialize.h"

namespace {

auto streamResultClass()
{
    return ::jxx::lang::ClassBase<
        ::jxx::ext::xml::transform::stream::StreamResult,
        ::jxx::lang::Object,
        ::jxx::ext::xml::transform::Result>::Class();
}

} // namespace

TEST(JxxXmlTransformStreamClassInitialization, StreamResultHasClassMetadata)
{
    ::jxx::initialize();

    EXPECT_NE(streamResultClass(), nullptr);
}

TEST(JxxXmlTransformStreamClassInitialization, RepeatedInitializationIsIdempotent)
{
    const auto before = streamResultClass();

    EXPECT_NO_THROW(
        ::jxx::ext::xml::transform::stream::initializeClasses());
    EXPECT_NO_THROW(
        ::jxx::ext::xml::transform::stream::initializeClasses());

    EXPECT_EQ(before, streamResultClass());
}
