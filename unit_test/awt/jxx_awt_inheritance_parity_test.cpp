#include <gtest/gtest.h>
#include <type_traits>
#include "awt/jxx.awt.Point.h"
#include "awt/geom/jxx.awt.geom.Line2D.h"
#include "awt/geom/jxx.awt.geom.Point2D.h"
#include "awt/geom/jxx.awt.geom.RectangularShape.h"
#include "awt/jxx.awt.Shape.h"
#include "io/jxx.io.SerializableI.h"
#include "lang/jxx.lang.Cloneable.h"

TEST(AwtInheritanceParity, PointUsesTheSourceSuperclassAndInterface) {
    static_assert(std::is_base_of_v<::jxx::awt::geom::Point2D, ::jxx::awt::Point>);
    static_assert(std::is_base_of_v<::jxx::io::SerializableI, ::jxx::awt::Point>);
    EXPECT_TRUE((std::is_base_of_v<::jxx::awt::geom::Point2D, ::jxx::awt::Point>));
}

TEST(AwtInheritanceParity, GeometryAbstractTypesExposeCloneableAndShape) {
    static_assert(std::is_base_of_v<::jxx::lang::Cloneable, ::jxx::awt::geom::Point2D>);
    static_assert(std::is_base_of_v<::jxx::lang::Cloneable, ::jxx::awt::geom::RectangularShape>);
    static_assert(std::is_base_of_v<::jxx::lang::Cloneable, ::jxx::awt::geom::Line2D>);
    static_assert(std::is_base_of_v<::jxx::awt::Shape, ::jxx::awt::geom::Line2D>);
    SUCCEED();
}
