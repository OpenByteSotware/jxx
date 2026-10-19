#include "awt/geom/jxx.awt.geom.ClassInitialization.h"

#include "awt/geom/jxx.awt.geom.AffineTransform.h"
#include "awt/geom/jxx.awt.geom.NoninvertibleTransformException.h"
#include "awt/geom/jxx.awt.geom.Point2D.h"
#include "io/jxx.io.SerializationClassRegistry.h"
#include "lang/jxx.lang.ClassInfo.h"

#include <mutex>

namespace jxx::awt::geom
{
    void initializeClasses()
    {
        static std::once_flag flag;
        std::call_once(flag, []()
        {
            ::jxx::lang::class_info_detail::registerClassInfo<AffineTransform>();
            ::jxx::lang::class_info_detail::registerClassInfo<Point2D>();
            ::jxx::lang::class_info_detail::registerClassInfo<Point2D::Float>();
            ::jxx::lang::class_info_detail::registerClassInfo<Point2D::Double>();
            (void)NoninvertibleTransformException::Class();

            ::jxx::io::serialization::registerSerializableClass<Point2D::Float>(
                static_cast<::jxx::lang::jlong>(-2870572449815403710LL));
            ::jxx::io::serialization::registerSerializableClass<Point2D::Double>(
                static_cast<::jxx::lang::jlong>(6150783262733311327LL));
        });
    }
}
