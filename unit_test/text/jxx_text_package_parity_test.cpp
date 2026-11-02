#include <gtest/gtest.h>
#include <type_traits>

#include "io/jxx.io.ByteArrayInputStream.h"
#include "io/jxx.io.ByteArrayOutputStream.h"
#include "io/jxx.io.ObjectInputStream.h"
#include "io/jxx.io.ObjectOutputStream.h"
#include "io/jxx.io.SerializableI.h"
#include "io/jxx.io.SerializationClassRegistry.h"
#include "lang/jxx.lang.Cloneable.h"
#include "lang/jxx.lang.String.h"
#include "text/jxx.text.ClassInitialization.h"
#include "text/jxx.text.DateFormat.h"
#include "text/jxx.text.DateFormatSymbols.h"
#include "text/jxx.text.DecimalFormat.h"
#include "text/jxx.text.Format.h"
#include "text/jxx.text.MessageFormat.h"
#include "text/jxx.text.NumberFormat.h"
#include "text/jxx.text.ParseException.h"
#include "text/jxx.text.ParsePosition.h"
#include "text/jxx.text.SimpleDateFormat.h"

namespace text = ::jxx::text;

TEST(TextPackageInheritanceParity, PublicHierarchyMatchesSourceTypes) {
    static_assert(std::is_base_of_v<::jxx::lang::Cloneable, text::Format>);
    static_assert(std::is_base_of_v<::jxx::io::SerializableI, text::Format>);
    static_assert(std::is_base_of_v<text::Format, text::DateFormat>);
    static_assert(std::is_base_of_v<text::Format, text::NumberFormat>);
    static_assert(std::is_base_of_v<text::NumberFormat, text::DecimalFormat>);
    static_assert(std::is_base_of_v<text::Format, text::MessageFormat>);
    static_assert(std::is_base_of_v<text::DateFormat, text::SimpleDateFormat>);
    static_assert(std::is_base_of_v<::jxx::lang::Cloneable, text::DateFormatSymbols>);
    static_assert(std::is_base_of_v<::jxx::io::SerializableI, text::DateFormatSymbols>);
    static_assert(!std::is_final_v<text::DateFormatSymbols>);
    SUCCEED();
}

TEST(TextPackageInitializationParity, RegistersImplementedPublicTypes) {
    text::initialize();
    EXPECT_NE(nullptr, text::Format::Class());
    EXPECT_NE(nullptr, text::DateFormat::Class());
    EXPECT_NE(nullptr, text::NumberFormat::Class());
    EXPECT_NE(nullptr, text::DecimalFormat::Class());
    EXPECT_NE(nullptr, text::MessageFormat::Class());
    EXPECT_NE(nullptr, text::SimpleDateFormat::Class());
    EXPECT_NE(nullptr, text::DateFormatSymbols::Class());
    EXPECT_NE(nullptr, text::ParseException::Class());
    EXPECT_NE(nullptr, text::ParsePosition::Class());
}

TEST(TextPackageSerializationParity, DateFormatSymbolsUsesSourceSerialVersionUID) {
    text::initialize();
    const auto metadata =
        ::jxx::io::serialization::SerializationClassRegistry::resolve(
            text::DateFormatSymbols::Class()->getName()->utf8());
    EXPECT_EQ(static_cast<::jxx::lang::jlong>(-5987973545549424702LL),
        metadata.serialVersionUID);
    EXPECT_TRUE(static_cast<bool>(metadata.deserializationFactory));
}

TEST(TextPackageSerializationParity, DateFormatSymbolsRoundTripPreservesCustomState) {
    text::initialize();
    auto original = ::jxx::NEW<text::DateFormatSymbols>();
    auto months = ::jxx::NEW<text::DateFormatSymbols::StringArray>(2);
    (*months)[0] = ::jxx::NEW<::jxx::lang::String>("First");
    (*months)[1] = ::jxx::NEW<::jxx::lang::String>("Second");
    original->setMonths(months);
    original->setLocalPatternChars(::jxx::NEW<::jxx::lang::String>("CUSTOM"));

    auto bytes = ::jxx::NEW<::jxx::io::ByteArrayOutputStream>();
    auto output = ::jxx::NEW<::jxx::io::ObjectOutputStream>(bytes);
    output->writeObject(::jxx::CAST<::jxx::lang::Object>(original));
    output->flush();

    auto inputBytes = ::jxx::NEW<::jxx::io::ByteArrayInputStream>(bytes->toByteArray());
    auto input = ::jxx::NEW<::jxx::io::ObjectInputStream>(inputBytes);
    auto restored = ::jxx::CAST<text::DateFormatSymbols>(input->readObject());

    ASSERT_NE(nullptr, restored);
    auto restoredMonths = restored->getMonths();
    ASSERT_NE(nullptr, restoredMonths);
    ASSERT_EQ(2, restoredMonths->length);
    EXPECT_EQ("First", (*restoredMonths)[0]->utf8());
    EXPECT_EQ("Second", (*restoredMonths)[1]->utf8());
    EXPECT_EQ("CUSTOM", restored->getLocalPatternChars()->utf8());
}
