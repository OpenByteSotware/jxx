#include <gtest/gtest.h>

#include "jxx.initialize.h"
#include "org/xml/sax/internal/jxx.org.xml.sax.internal.ClassInitialization.h"
#include "org/xml/sax/internal/jxx.org.xml.sax.internal.PugiXMLReader.h"

namespace {

auto pugiXMLReaderClass()
{
    return ::jxx::lang::ClassBase<
        ::jxx::org::xml::sax::internal::PugiXMLReader,
        ::jxx::lang::Object,
        ::jxx::org::xml::sax::XMLReader>::Class();
}

} // namespace

TEST(JxxSaxInternalClassInitialization, PugiXMLReaderHasClassMetadata)
{
    ::jxx::initialize();

    EXPECT_NE(pugiXMLReaderClass(), nullptr);
}

TEST(JxxSaxInternalClassInitialization, RepeatedInitializationIsIdempotent)
{
    const auto before = pugiXMLReaderClass();

    EXPECT_NO_THROW(::jxx::org::xml::sax::internal::initializeClasses());
    EXPECT_NO_THROW(::jxx::org::xml::sax::internal::initializeClasses());

    EXPECT_EQ(before, pugiXMLReaderClass());
}
