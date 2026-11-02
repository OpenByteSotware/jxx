#include <gtest/gtest.h>
#include <type_traits>

#include "io/jxx.io.ByteArrayInputStream.h"
#include "io/jxx.io.ByteArrayOutputStream.h"
#include "io/jxx.io.ObjectInputStream.h"
#include "io/jxx.io.ObjectOutputStream.h"
#include "io/jxx.io.SerializableI.h"
#include "io/jxx.io.SerializationClassRegistry.h"
#include "lang/jxx.lang.IllegalArgumentException.h"
#include "lang/jxx.lang.String.h"
#include "util/regex/jxx.util.regex.ClassInitialization.h"
#include "util/regex/jxx.util.regex.MatchResult.h"
#include "util/regex/jxx.util.regex.Matcher.h"
#include "util/regex/jxx.util.regex.Pattern.h"
#include "util/regex/jxx.util.regex.PatternSyntaxException.h"

namespace regex = ::jxx::util::regex;

TEST(RegexPackageParity, PublicInheritanceMatchesSourceTypes) {
    static_assert(std::is_base_of_v<::jxx::io::SerializableI, regex::Pattern>);
    static_assert(std::is_base_of_v<regex::MatchResult, regex::Matcher>);
    static_assert(std::is_base_of_v<::jxx::lang::IllegalArgumentException,
        regex::PatternSyntaxException>);
    SUCCEED();
}

TEST(RegexPackageParity, PackageInitializationRegistersAllPublicTypes) {
    regex::initialize();
    EXPECT_NE(nullptr, regex::MatchResult::Class());
    EXPECT_NE(nullptr, regex::Matcher::Class());
    EXPECT_NE(nullptr, regex::Pattern::Class());
    EXPECT_NE(nullptr, regex::PatternSyntaxException::Class());
}

TEST(RegexPackageParity, PatternSerializationRegistrationUsesSourceUID) {
    regex::initialize();
    const auto metadata =
        ::jxx::io::serialization::SerializationClassRegistry::resolve(
            regex::Pattern::Class()->getName()->utf8());
    EXPECT_EQ(static_cast<::jxx::lang::jlong>(5073258162644648461LL),
        metadata.serialVersionUID);
    EXPECT_TRUE(static_cast<bool>(metadata.deserializationFactory));
}

TEST(RegexPackageParity, PatternSerializationRoundTripPreservesPatternAndFlags) {
    regex::initialize();
    auto original = regex::Pattern::compile(
        ::jxx::NEW<::jxx::lang::String>("a+b"),
        regex::Pattern::CASE_INSENSITIVE | regex::Pattern::MULTILINE);

    auto bytes = ::jxx::NEW<::jxx::io::ByteArrayOutputStream>();
    auto output = ::jxx::NEW<::jxx::io::ObjectOutputStream>(bytes);
    output->writeObject(::jxx::CAST<::jxx::lang::Object>(original));
    output->flush();

    auto inputBytes = ::jxx::NEW<::jxx::io::ByteArrayInputStream>(bytes->toByteArray());
    auto input = ::jxx::NEW<::jxx::io::ObjectInputStream>(inputBytes);
    auto restored = ::jxx::CAST<regex::Pattern>(input->readObject());

    ASSERT_NE(nullptr, restored);
    EXPECT_EQ("a+b", restored->pattern()->utf8());
    EXPECT_EQ(regex::Pattern::CASE_INSENSITIVE | regex::Pattern::MULTILINE,
        restored->flags());
}
