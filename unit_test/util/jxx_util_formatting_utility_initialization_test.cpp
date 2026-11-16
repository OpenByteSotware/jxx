#include <gtest/gtest.h>

#include "lang/jxx.lang.Object.h"
#include "util/jxx.util.Base64.h"
#include "util/jxx.util.ClassInitialization.h"
#include "util/jxx.util.Formatter.h"
#include "util/jxx.util.Optional.h"
#include "util/jxx.util.OptionalDouble.h"
#include "util/jxx.util.OptionalInt.h"
#include "util/jxx.util.OptionalLong.h"
#include "util/jxx.util.Scanner.h"
#include "util/jxx.util.StringJoiner.h"
#include "util/jxx.util.StringTokenizer.h"

TEST(UtilFormattingUtilityInitialization, RegistersPublicUtilityTypes) {
    ::jxx::util::initializeClasses();

    using Object = ::jxx::lang::Object;
    EXPECT_NE(nullptr, ::jxx::util::Base64::Class());
    EXPECT_NE(nullptr, ::jxx::util::Base64::Encoder::Class());
    EXPECT_NE(nullptr, ::jxx::util::Base64::Decoder::Class());
    EXPECT_NE(nullptr, ::jxx::util::Formatter::Class());
    EXPECT_NE(nullptr, ::jxx::util::Scanner::Class());
    EXPECT_NE(nullptr, ::jxx::util::StringJoiner::Class());
    EXPECT_NE(nullptr, ::jxx::util::StringTokenizer::Class());
    EXPECT_NE(nullptr, ::jxx::util::Optional<Object>::Class());
    EXPECT_NE(nullptr, ::jxx::util::OptionalInt::Class());
    EXPECT_NE(nullptr, ::jxx::util::OptionalLong::Class());
    EXPECT_NE(nullptr, ::jxx::util::OptionalDouble::Class());
}

TEST(UtilBase64NestedTypes, FactoriesReturnInitializedPublicTypes) {
    ::jxx::util::initializeClasses();
    EXPECT_NE(nullptr, ::jxx::util::Base64::getEncoder());
    EXPECT_NE(nullptr, ::jxx::util::Base64::getDecoder());
    EXPECT_NE(nullptr, ::jxx::util::Base64::getUrlEncoder());
    EXPECT_NE(nullptr, ::jxx::util::Base64::getUrlDecoder());
    EXPECT_NE(nullptr, ::jxx::util::Base64::getMimeEncoder());
    EXPECT_NE(nullptr, ::jxx::util::Base64::getMimeDecoder());
}
