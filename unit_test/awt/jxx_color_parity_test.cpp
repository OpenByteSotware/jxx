#include <gtest/gtest.h>

#include "awt/jxx.awt.Color.h"
#include "awt/jxx.awt.Paint.h"
#include "awt/jxx.awt.Transparency.h"
#include "io/jxx.io.ByteArrayInputStream.h"
#include "io/jxx.io.ByteArrayOutputStream.h"
#include "io/jxx.io.ObjectInputStream.h"
#include "io/jxx.io.ObjectOutputStream.h"
#include "io/jxx.io.SerializableI.h"
#include "lang/jxx.lang.Object.h"

TEST(ColorParityTest, ImplementsPaintAndSerializable)
{
    const auto color = ::jxx::NEW<::jxx::awt::Color>(1, 2, 3, 4);
    EXPECT_NE(::jxx::CAST<::jxx::awt::Paint>(color), nullptr);
    EXPECT_NE(::jxx::CAST<::jxx::io::SerializableI>(color), nullptr);
}

TEST(ColorParityTest, ReportsTransparencyFromAlpha)
{
    EXPECT_EQ(::jxx::NEW<::jxx::awt::Color>(1, 2, 3, 255)->getTransparency(),
              ::jxx::awt::Transparency::OPAQUE);
    EXPECT_EQ(::jxx::NEW<::jxx::awt::Color>(1, 2, 3, 0)->getTransparency(),
              ::jxx::awt::Transparency::BITMASK);
    EXPECT_EQ(::jxx::NEW<::jxx::awt::Color>(1, 2, 3, 127)->getTransparency(),
              ::jxx::awt::Transparency::TRANSLUCENT);
}

TEST(ColorParityTest, SerializationRoundTripPreservesArgb)
{
    const auto bytes = ::jxx::NEW<::jxx::io::ByteArrayOutputStream>();
    const auto output = ::jxx::NEW<::jxx::io::ObjectOutputStream>(bytes);
    const auto original = ::jxx::NEW<::jxx::awt::Color>(10, 20, 30, 40);
    output->writeObject(::jxx::CAST<::jxx::lang::Object>(original));
    output->flush();

    const auto input = ::jxx::NEW<::jxx::io::ObjectInputStream>(
        ::jxx::NEW<::jxx::io::ByteArrayInputStream>(bytes->toByteArray()));
    const auto restored = ::jxx::CAST<::jxx::awt::Color>(input->readObject());

    ASSERT_NE(restored, nullptr);
    EXPECT_EQ(restored->getRGB(), original->getRGB());
    EXPECT_EQ(restored->getAlpha(), 40);
}
