#include <gtest/gtest.h>
#include <type_traits>

#include "awt/geom/jxx.awt.geom.AffineTransform.h"
#include "awt/geom/jxx.awt.geom.ClassInitialization.h"
#include "awt/geom/jxx.awt.geom.Dimension2D.h"
#include "awt/geom/jxx.awt.geom.Rectangle2D.h"
#include "awt/geom/jxx.awt.geom.RectangularShape.h"
#include "io/jxx.io.ByteArrayInputStream.h"
#include "io/jxx.io.ByteArrayOutputStream.h"
#include "io/jxx.io.ObjectInputStream.h"
#include "io/jxx.io.ObjectOutputStream.h"
#include "io/jxx.io.SerializableI.h"
#include "io/jxx.io.SerializationClassRegistry.h"
#include "lang/jxx.lang.Cloneable.h"

TEST(AwtGeomPackageParity, PublicHierarchyMatchesSourceTypes) {
    static_assert(std::is_base_of_v<::jxx::awt::geom::RectangularShape,
        ::jxx::awt::geom::Rectangle2D>);
    static_assert(std::is_base_of_v<::jxx::lang::Cloneable,
        ::jxx::awt::geom::Dimension2D>);
    static_assert(std::is_base_of_v<::jxx::lang::Cloneable,
        ::jxx::awt::geom::AffineTransform>);
    static_assert(std::is_base_of_v<::jxx::io::SerializableI,
        ::jxx::awt::geom::AffineTransform>);
    SUCCEED();
}

TEST(AwtGeomPackageParity, PackageInitializationRegistersCorrectMetadata) {
    ::jxx::awt::geom::initialize();
    EXPECT_NE(nullptr, ::jxx::awt::geom::Rectangle2D::Class());
    EXPECT_NE(nullptr, ::jxx::awt::geom::RectangularShape::Class());
    EXPECT_NE(nullptr, ::jxx::awt::geom::AffineTransform::Class());
    EXPECT_NE(nullptr, ::jxx::awt::geom::Dimension2D::Class());
}

TEST(AwtGeomPackageParity, AffineTransformSerializationRegistrationUsesSourceUID) {
    ::jxx::awt::geom::initialize();
    const auto metadata =
        ::jxx::io::serialization::SerializationClassRegistry::resolve(
            ::jxx::awt::geom::AffineTransform::Class()->getName()->utf8());
    EXPECT_EQ(static_cast<::jxx::lang::jlong>(1330973210523860834LL),
        metadata.serialVersionUID);
    EXPECT_TRUE(static_cast<bool>(metadata.deserializationFactory));
}

TEST(AwtGeomPackageParity, AffineTransformSerializationRoundTripPreservesMatrix) {
    ::jxx::awt::geom::initialize();
    auto original = ::jxx::NEW<::jxx::awt::geom::AffineTransform>(1.0, 2.0, 3.0, 4.0, 5.0, 6.0);
    auto bytes = ::jxx::NEW<::jxx::io::ByteArrayOutputStream>();
    auto output = ::jxx::NEW<::jxx::io::ObjectOutputStream>(bytes);
    output->writeObject(::jxx::CAST<::jxx::lang::Object>(original));
    output->flush();
    auto inputBytes = ::jxx::NEW<::jxx::io::ByteArrayInputStream>(bytes->toByteArray());
    auto input = ::jxx::NEW<::jxx::io::ObjectInputStream>(inputBytes);
    auto restored = ::jxx::CAST<::jxx::awt::geom::AffineTransform>(input->readObject());
    ASSERT_NE(nullptr, restored);
    auto matrix = ::jxx::NEW<::jxx::lang::JxxArray<::jxx::lang::jdouble, 1>>(6);
    restored->getMatrix(matrix);
    EXPECT_DOUBLE_EQ(1.0, (*matrix)[0]);
    EXPECT_DOUBLE_EQ(2.0, (*matrix)[1]);
    EXPECT_DOUBLE_EQ(3.0, (*matrix)[2]);
    EXPECT_DOUBLE_EQ(4.0, (*matrix)[3]);
    EXPECT_DOUBLE_EQ(5.0, (*matrix)[4]);
    EXPECT_DOUBLE_EQ(6.0, (*matrix)[5]);
}
