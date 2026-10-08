#include <gtest/gtest.h>
#include "util/jxx.util.Optional.h"
#include "lang/jxx.lang.String.h"

TEST(JxxOptionalJava8ParityTest, EmptyAndPresentBehavior) {
    using OptionalString = ::jxx::util::Optional<::jxx::lang::String>;
    const auto empty = OptionalString::empty();
    EXPECT_FALSE(empty->isPresent());
    EXPECT_THROW(empty->get(), ::jxx::util::NoSuchElementException);
    EXPECT_EQ(empty->hashCode(), 0);
    EXPECT_EQ(empty->toString()->utf8(), "Optional.empty");

    const auto value = ::jxx::NEW<::jxx::lang::String>("value");
    const auto present = OptionalString::of(value);
    EXPECT_TRUE(present->isPresent());
    EXPECT_EQ(present->get(), value);
    EXPECT_EQ(present->orElse(nullptr), value);
    EXPECT_EQ(present->toString()->utf8(), "Optional[value]");
}

TEST(JxxOptionalJava8ParityTest, OfRejectsNullAndNullableReturnsEmpty) {
    using OptionalString = ::jxx::util::Optional<::jxx::lang::String>;
    EXPECT_THROW(OptionalString::of(nullptr), ::jxx::lang::NullPointerException);
    EXPECT_FALSE(OptionalString::ofNullable(nullptr)->isPresent());
}
