#include <gtest/gtest.h>
#include "jxx.initialize.h"
#include "nio/charset/jxx.nio.charset.ClassInitialization.h"
#include "nio/charset/jxx.nio.charset.Charset.h"
#include "nio/charset/jxx.nio.charset.CharsetDecoder.h"
#include "nio/charset/jxx.nio.charset.CharsetEncoder.h"
#include "nio/charset/jxx.nio.charset.CoderResult.h"
#include "nio/charset/jxx.nio.charset.CodingErrorAction.h"
#include "nio/charset/jxx.nio.charset.StandardCharsets.h"
TEST(JxxNioCharsetClassInitialization, MetadataTypesAreInitialized)
{
    namespace C = ::jxx::nio::charset;
    ::jxx::initialize();
    EXPECT_NE((::jxx::lang::ClassBase<C::Charset, ::jxx::lang::Object>::Class()), nullptr);
    EXPECT_NE((::jxx::lang::ClassBase<C::CharsetDecoder, ::jxx::lang::Object>::Class()), nullptr);
    EXPECT_NE((::jxx::lang::ClassBase<C::CharsetEncoder, ::jxx::lang::Object>::Class()), nullptr);
    EXPECT_NE((::jxx::lang::ClassBase<C::CoderResult, ::jxx::lang::Object>::Class()), nullptr);
    EXPECT_NE((::jxx::lang::ClassBase<C::CodingErrorAction, ::jxx::lang::Object>::Class()), nullptr);
    EXPECT_NE((::jxx::lang::ClassBase<C::StandardCharsets, ::jxx::lang::Object>::Class()), nullptr);
}
TEST(JxxNioCharsetClassInitialization, RepeatedInitializationIsIdempotent)
{
    namespace C = ::jxx::nio::charset;
    const auto before = ::jxx::lang::ClassBase<C::Charset, ::jxx::lang::Object>::Class();
    EXPECT_NO_THROW(C::initializeClasses());
    EXPECT_NO_THROW(C::initializeClasses());
    EXPECT_EQ(before, (::jxx::lang::ClassBase<C::Charset, ::jxx::lang::Object>::Class()));
}
