#include "awt/jxx.awt.AWTPermission.h"

namespace jxx::awt
{
    AWTPermission::AWTPermission(
        const ::jxx::Ptr<::jxx::lang::String>& name) : Super(name)
    {
    }

    AWTPermission::AWTPermission(
        const ::jxx::Ptr<::jxx::lang::String>& name,
        const ::jxx::Ptr<::jxx::lang::String>& actions)
        : Super(name, actions)
    {
    }
}
