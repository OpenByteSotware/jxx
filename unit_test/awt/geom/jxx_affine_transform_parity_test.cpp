#include <gtest/gtest.h>

#include "jxx.initialize.h"
#include "awt/geom/jxx.awt.geom.AffineTransform.h"
#include "awt/geom/jxx.awt.geom.NoninvertibleTransformException.h"
#include "awt/geom/jxx.awt.geom.Point2D.h"

#include <cmath>

TEST(AffineTransformParity, RotateNinetyDegrees)
{
    ::jxx::initialize();
    auto transform = ::jxx::awt::geom::AffineTransform::getRotateInstance(
        3.14159265358979323846 / 2.0);
    auto source = ::jxx::NEW<::jxx::awt::geom::Point2D::Double>(1.0, 0.0);
    auto result = transform->transform(source, nullptr);
    ASSERT_NE(result, nullptr);
    EXPECT_NEAR(result->getX(), 0.0, 1.0e-12);
    EXPECT_NEAR(result->getY(), 1.0, 1.0e-12);
}

TEST(AffineTransformParity, ConcatenationUsesExpectedOrder)
{
    auto transform = ::jxx::awt::geom::AffineTransform::getTranslateInstance(10.0, 20.0);
    transform->concatenate(
        ::jxx::awt::geom::AffineTransform::getScaleInstance(2.0, 3.0));
    auto source = ::jxx::NEW<::jxx::awt::geom::Point2D::Double>(4.0, 5.0);
    auto result = transform->transform(source, nullptr);
    EXPECT_DOUBLE_EQ(result->getX(), 18.0);
    EXPECT_DOUBLE_EQ(result->getY(), 35.0);
}

TEST(AffineTransformParity, InverseRestoresPoint)
{
    auto transform = ::jxx::NEW<::jxx::awt::geom::AffineTransform>();
    transform->translate(7.0, -4.0);
    transform->rotate(0.35);
    transform->scale(2.0, 3.0);
    auto inverse = transform->createInverse();
    auto source = ::jxx::NEW<::jxx::awt::geom::Point2D::Double>(9.0, 11.0);
    auto transformed = transform->transform(source, nullptr);
    auto restored = inverse->transform(transformed, nullptr);
    EXPECT_NEAR(restored->getX(), source->getX(), 1.0e-10);
    EXPECT_NEAR(restored->getY(), source->getY(), 1.0e-10);
}

TEST(AffineTransformParity, SingularTransformCannotBeInverted)
{
    auto transform = ::jxx::awt::geom::AffineTransform::getScaleInstance(0.0, 1.0);
    EXPECT_THROW(transform->invert(),
        ::jxx::awt::geom::NoninvertibleTransformException);
}

TEST(AffineTransformParity, MatrixUsesSixElementOrdering)
{
    auto transform = ::jxx::NEW<::jxx::awt::geom::AffineTransform>(1, 2, 3, 4, 5, 6);
    auto matrix = ::jxx::NEW<::jxx::lang::JxxArray<::jxx::lang::jdouble, 1>>(6);
    transform->getMatrix(matrix);
    EXPECT_DOUBLE_EQ((*matrix)[0], 1.0);
    EXPECT_DOUBLE_EQ((*matrix)[1], 2.0);
    EXPECT_DOUBLE_EQ((*matrix)[2], 3.0);
    EXPECT_DOUBLE_EQ((*matrix)[3], 4.0);
    EXPECT_DOUBLE_EQ((*matrix)[4], 5.0);
    EXPECT_DOUBLE_EQ((*matrix)[5], 6.0);
}
