#include <gtest/gtest.h>
#include <type_traits>

#include "lang/jxx.lang.Comparable.h"
#include "lang/jxx.lang.String.h"
#include "nio/charset/jxx.nio.charset.Charset.h"
#include "nio/charset/jxx.nio.charset.CharsetDecoder.h"
#include "nio/charset/jxx.nio.charset.CharsetEncoder.h"
#include "nio/charset/jxx.nio.charset.ClassInitialization.h"
#include "nio/charset/jxx.nio.charset.CoderResult.h"
#include "nio/charset/jxx.nio.charset.CodingErrorAction.h"
#include "nio/charset/jxx.nio.charset.StandardCharsets.h"

namespace charset = ::jxx::nio::charset;

TEST(CharsetPackageInheritanceParity, CharsetImplementsComparable) {
    static_assert(std::is_base_of_v<
        ::jxx::lang::Comparable<charset::Charset>, charset::Charset>);
    SUCCEED();
}

TEST(CharsetPackageInitializationParity, RegistersImplementedPublicTypes) {
    charset::initialize();
    EXPECT_NE(nullptr, charset::Charset::Class());
    EXPECT_NE(nullptr, charset::CharsetDecoder::Class());
    EXPECT_NE(nullptr, charset::CharsetEncoder::Class());
    EXPECT_NE(nullptr, charset::CoderResult::Class());
    EXPECT_NE(nullptr, charset::CodingErrorAction::Class());
    EXPECT_NE(nullptr, charset::StandardCharsets::Class());
}

TEST(CharsetPackageBehaviorParity, CompareToUsesCaseInsensitiveCanonicalNameOrder) {
    auto lower = ::jxx::NEW<charset::Charset>(
        ::jxx::NEW<::jxx::lang::String>("utf-8"),
        ::jxx::NEW<::jxx::lang::JxxArray<::jxx::Ptr<::jxx::lang::String>, 1U>>(0));
    auto upper = ::jxx::NEW<charset::Charset>(
        ::jxx::NEW<::jxx::lang::String>("UTF-8"),
        ::jxx::NEW<::jxx::lang::JxxArray<::jxx::Ptr<::jxx::lang::String>, 1U>>(0));
    EXPECT_EQ(0, lower->compareTo(upper));
}
