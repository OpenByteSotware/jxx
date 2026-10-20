#include "awt/jxx.awt.ClassInitialization.h"

#include "awt/jxx.awt.Component.h"
#include "awt/jxx.awt.Container.h"
#include "awt/jxx.awt.Polygon.h"
#include "awt/jxx.awt.Toolkit.h"
#include "io/jxx.io.ArrayCodecRegistry.h"
#include "io/jxx.io.SerializationClassRegistry.h"

#include <mutex>

namespace jxx::awt
{
    void initializeClasses()
    {
        static std::once_flag flag;
        std::call_once(flag, []()
        {
            (void)::jxx::awt::Component::Class();
            (void)::jxx::awt::Container::Class();
            (void)::jxx::awt::Polygon::Class();
            ::jxx::io::serialization::registerSerializableClass<
                ::jxx::awt::Polygon>(
                    static_cast<::jxx::lang::jlong>(-6460061437900069969LL));
            ::jxx::io::serialization::registerClassReferenceArrayCodecs<
                ::jxx::awt::Polygon, 1U, 2U>();
            (void)::jxx::awt::Toolkit::Class();
        });
    }
}
