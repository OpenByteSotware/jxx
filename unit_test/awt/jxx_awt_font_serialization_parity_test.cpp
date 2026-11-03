#include <type_traits>

#include <gtest/gtest.h>

#include "awt/jxx.awt.ClassInitialization.h"
#include "awt/jxx.awt.Font.h"
#include "io/jxx.io.ByteArrayInputStream.h"
#include "io/jxx.io.ByteArrayOutputStream.h"
#include "io/jxx.io.ObjectInputStream.h"
#include "io/jxx.io.ObjectOutputStream.h"
#include "io/jxx.io.SerializableI.h"
#include "lang/jxx.lang.String.h"

namespace {

TEST(AwtFontJava8Parity, ImplementsSerializable) {
    EXPECT_TRUE((std::is_base_of_v<
        ::jxx::io::SerializableI,
        ::jxx::awt::Font>));
}

TEST(AwtFontJava8Parity, SerializationRoundTripPreservesState) {
    ::jxx::awt::initializeClasses();

    const auto original = ::jxx::NEW<::jxx::awt::Font>(
        ::jxx::NEW<::jxx::lang::String>("Dialog"),
        ::jxx::awt::Font::BOLD | ::jxx::awt::Font::ITALIC,
        14)->deriveFont(14.5F);

    const auto bytes = ::jxx::NEW<::jxx::io::ByteArrayOutputStream>();
    const auto output = ::jxx::NEW<::jxx::io::ObjectOutputStream>(bytes);
    output->writeObject(original);
    output->flush();

    const auto source = ::jxx::NEW<::jxx::io::ByteArrayInputStream>(
        bytes->toByteArray());
    const auto input = ::jxx::NEW<::jxx::io::ObjectInputStream>(source);
    const auto restored = ::jxx::CAST<::jxx::awt::Font>(input->readObject());

    ASSERT_NE(nullptr, restored);
    EXPECT_EQ("Dialog", restored->getName()->utf8());
    EXPECT_EQ(::jxx::awt::Font::BOLD | ::jxx::awt::Font::ITALIC,
              restored->getStyle());
    EXPECT_FLOAT_EQ(14.5F, restored->getSize2D());
}

} // namespace
