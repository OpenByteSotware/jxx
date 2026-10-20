#include <gtest/gtest.h>

#include "awt/geom/jxx.awt.geom.ClassInitialization.h"
#include "awt/geom/jxx.awt.geom.Dimension2D.h"
#include "awt/geom/jxx.awt.geom.Ellipse2D.h"
#include "awt/geom/jxx.awt.geom.IllegalPathStateException.h"
#include "awt/geom/jxx.awt.geom.Line2D.h"
#include "awt/geom/jxx.awt.geom.Rectangle2D.h"
#include "awt/geom/jxx.awt.geom.RectangularShape.h"

namespace
{
    template <typename T>
    void expectClassMetadata()
    {
        const auto type = T::Class();
        ASSERT_NE(type, nullptr);
        ASSERT_NE(type->getName(), nullptr);
        EXPECT_FALSE(type->getName()->utf8().empty());
    }

    TEST(AwtGeomClassInitialization, RegistersExistingGeometryClasses)
    {
        ::jxx::awt::geom::initializeClasses();
        expectClassMetadata<::jxx::awt::geom::Dimension2D>();
        expectClassMetadata<::jxx::awt::geom::Ellipse2D>();
        expectClassMetadata<::jxx::awt::geom::Ellipse2D::Double>();
        expectClassMetadata<::jxx::awt::geom::IllegalPathStateException>();
        expectClassMetadata<::jxx::awt::geom::Line2D>();
        expectClassMetadata<::jxx::awt::geom::Line2D::Double>();
        expectClassMetadata<::jxx::awt::geom::Rectangle2D>();
        expectClassMetadata<::jxx::awt::geom::Rectangle2D::Double>();
        expectClassMetadata<::jxx::awt::geom::RectangularShape>();
    }
}
