#include <gtest/gtest.h>

#include "io/jxx.io.ByteArrayInputStream.h"
#include "io/jxx.io.ByteArrayOutputStream.h"
#include "io/jxx.io.ObjectInputStream.h"
#include "io/jxx.io.ObjectOutputStream.h"
#include "jxx.initialize.h"
#include "lang/jxx.lang.CharSequence.h"
#include "lang/jxx.lang.String.h"
#include "lang/jxx.lang.buildin_array.h"
#include "util/regex/jxx.util.regex.Pattern.h"
#include "util/regex/jxx.util.regex.Matcher.h"

namespace {

using Pattern = ::jxx::util::regex::Pattern;
using String = ::jxx::lang::String;
using PatternArray =
    ::jxx::lang::JxxArray<::jxx::Ptr<Pattern>, 1U>;
using PatternMatrix =
    ::jxx::lang::JxxArray<::jxx::Ptr<Pattern>, 2U>;

template <typename T>
::jxx::Ptr<T> roundTrip(const ::jxx::Ptr<T>& value) {
    auto bytes = ::jxx::NEW<::jxx::io::ByteArrayOutputStream>();
    {
        auto output = ::jxx::NEW<::jxx::io::ObjectOutputStream>(bytes);
        output->writeObject(::jxx::CAST<::jxx::lang::Object>(value));
        output->flush();
    }
    auto byteInput =
        ::jxx::NEW<::jxx::io::ByteArrayInputStream>(bytes->toByteArray());
    auto input = ::jxx::NEW<::jxx::io::ObjectInputStream>(byteInput);
    return ::jxx::CAST<T>(input->readObject());
}

TEST(PatternSerializationInitialization, RoundTripsExpressionAndFlags) {
    ::jxx::initialize();
    const auto expression = ::jxx::NEW<String>("^device-[0-9]+$");
    const auto original = Pattern::compile(
        expression,
        Pattern::CASE_INSENSITIVE | Pattern::MULTILINE);

    const auto restored = roundTrip(original);

    ASSERT_NE(restored, nullptr);
    EXPECT_EQ(expression->utf8(), restored->pattern()->utf8());
    EXPECT_EQ(original->flags(), restored->flags());
    EXPECT_TRUE(restored->matcher(
        ::jxx::CAST<::jxx::lang::CharSequence>(
            ::jxx::NEW<String>("DEVICE-42")))->matches());
}

TEST(PatternSerializationInitialization, RoundTripsRankOnePatternArray) {
    ::jxx::initialize();
    auto original = ::jxx::NEW<PatternArray>(2);
    (*original)[0] = Pattern::compile(::jxx::NEW<String>("alpha"));
    (*original)[1] = Pattern::compile(::jxx::NEW<String>("beta"));

    auto restored = roundTrip(original);

    ASSERT_NE(restored, nullptr);
    ASSERT_EQ(2, restored->length);
    EXPECT_EQ("alpha", (*restored)[0]->pattern()->utf8());
    EXPECT_EQ("beta", (*restored)[1]->pattern()->utf8());
}

TEST(PatternSerializationInitialization, RoundTripsRankTwoPatternArray) {
    ::jxx::initialize();
    using Row = PatternMatrix::SubArray;
    auto original = ::jxx::NEW<PatternMatrix>(1);
    auto row = ::jxx::NEW<Row>(2);
    (*row)[0] = Pattern::compile(::jxx::NEW<String>("one"));
    (*row)[1] = Pattern::compile(::jxx::NEW<String>("two"));
    (*original)[0] = row;

    auto restored = roundTrip(original);

    ASSERT_NE(restored, nullptr);
    ASSERT_EQ(1, restored->length);
    const auto& restoredRow = restored->row(0);
    ASSERT_NE(restoredRow, nullptr);
    EXPECT_EQ("one", (*restoredRow)[0]->pattern()->utf8());
    EXPECT_EQ("two", (*restoredRow)[1]->pattern()->utf8());
}

} // namespace
