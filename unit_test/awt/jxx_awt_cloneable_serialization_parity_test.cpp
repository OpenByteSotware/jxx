#include <type_traits>

#include <gtest/gtest.h>

#include "awt/jxx.awt.BufferCapabilities.h"
#include "awt/jxx.awt.ClassInitialization.h"
#include "awt/jxx.awt.ImageCapabilities.h"
#include "awt/jxx.awt.Insets.h"
#include "awt/jxx.awt.Point.h"
#include "io/jxx.io.ByteArrayInputStream.h"
#include "io/jxx.io.ByteArrayOutputStream.h"
#include "io/jxx.io.ObjectInputStream.h"
#include "io/jxx.io.ObjectOutputStream.h"
#include "io/jxx.io.SerializableI.h"
#include "lang/jxx.lang.Cloneable.h"

namespace {

TEST(AwtJava8HierarchyParity, CapabilitiesAreCloneable) {
    EXPECT_TRUE((std::is_base_of_v<
        ::jxx::lang::Cloneable, ::jxx::awt::BufferCapabilities>));
    EXPECT_TRUE((std::is_base_of_v<
        ::jxx::lang::Cloneable, ::jxx::awt::ImageCapabilities>));
}

TEST(AwtJava8HierarchyParity, InsetsIsCloneableAndSerializable) {
    EXPECT_TRUE((std::is_base_of_v<
        ::jxx::lang::Cloneable, ::jxx::awt::Insets>));
    EXPECT_TRUE((std::is_base_of_v<
        ::jxx::io::SerializableI, ::jxx::awt::Insets>));
}

TEST(AwtJava8HierarchyParity, InsetsCloneCopiesState) {
    const auto source = ::jxx::NEW<::jxx::awt::Insets>(1, 2, 3, 4);
    const auto clone = ::jxx::CAST<::jxx::awt::Insets>(source->clone());

    ASSERT_NE(nullptr, clone);
    EXPECT_NE(source.get(), clone.get());
    EXPECT_EQ(1, clone->top);
    EXPECT_EQ(2, clone->left);
    EXPECT_EQ(3, clone->bottom);
    EXPECT_EQ(4, clone->right);
}

TEST(AwtJava8SerializationParity, InsetsRoundTripPreservesState) {
    ::jxx::awt::initializeClasses();

    const auto bytes = ::jxx::NEW<::jxx::io::ByteArrayOutputStream>();
    const auto output = ::jxx::NEW<::jxx::io::ObjectOutputStream>(bytes);
    output->writeObject(::jxx::NEW<::jxx::awt::Insets>(5, 6, 7, 8));
    output->flush();

    const auto inputBytes = ::jxx::NEW<::jxx::io::ByteArrayInputStream>(
        bytes->toByteArray());
    const auto input = ::jxx::NEW<::jxx::io::ObjectInputStream>(inputBytes);
    const auto restored = ::jxx::CAST<::jxx::awt::Insets>(input->readObject());

    ASSERT_NE(nullptr, restored);
    EXPECT_EQ(5, restored->top);
    EXPECT_EQ(6, restored->left);
    EXPECT_EQ(7, restored->bottom);
    EXPECT_EQ(8, restored->right);
}

TEST(AwtJava8SerializationParity, PointRoundTripPreservesState) {
    ::jxx::awt::initializeClasses();

    const auto bytes = ::jxx::NEW<::jxx::io::ByteArrayOutputStream>();
    const auto output = ::jxx::NEW<::jxx::io::ObjectOutputStream>(bytes);
    output->writeObject(::jxx::NEW<::jxx::awt::Point>(9, 10));
    output->flush();

    const auto inputBytes = ::jxx::NEW<::jxx::io::ByteArrayInputStream>(
        bytes->toByteArray());
    const auto input = ::jxx::NEW<::jxx::io::ObjectInputStream>(inputBytes);
    const auto restored = ::jxx::CAST<::jxx::awt::Point>(input->readObject());

    ASSERT_NE(nullptr, restored);
    EXPECT_EQ(9, restored->x);
    EXPECT_EQ(10, restored->y);
}

} // namespace
