#include "awt/jxx.awt.ClassInitialization.h"

#include "awt/jxx.awt.Component.h"
#include "awt/jxx.awt.Container.h"

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
        });
    }
}
