#include <gtest/gtest.h>
#include "lang/jxx.lang.ClassInfo.h"
#include "jxx.initialize.h"
#include "org/w3c/dom/ls/jxx.org.w3c.dom.ls.ClassInitialization.h"
#include "org/w3c/dom/ls/jxx.org.w3c.dom.ls.LSException.h"

TEST(JxxDomLsClassInitialization, LSExceptionHasClassMetadata)
{
    ::jxx::initialize();

    EXPECT_NE(
        ::jxx::org::w3c::dom::ls::LSException::JxxClassInfoMarker::Class(),
        nullptr);
}

TEST(JxxDomLsClassInitialization, RepeatedInitializationIsIdempotent)
{
    const auto before =
        ::jxx::org::w3c::dom::ls::LSException::JxxClassInfoMarker::Class();

    EXPECT_NO_THROW(::jxx::org::w3c::dom::ls::initializeClasses());
    EXPECT_NO_THROW(::jxx::org::w3c::dom::ls::initializeClasses());

    EXPECT_EQ(
        before,
        ::jxx::org::w3c::dom::ls::LSException::JxxClassInfoMarker::Class());
}
