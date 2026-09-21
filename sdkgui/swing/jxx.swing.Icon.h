#pragma once

#include "lang/jxx.lang.ClassInfo.h"
#include "lang/jxx_types.h"

namespace jxx::awt
{
    class Component;
    class Graphics;
}

namespace jxx::swing
{
    class Icon : public ::jxx::lang::InterfaceBase<Icon>
    {
    public:
        ~Icon() override = default;

        virtual void paintIcon(
            const ::jxx::Ptr<::jxx::awt::Component>& component,
            const ::jxx::Ptr<::jxx::awt::Graphics>& graphics,
            ::jxx::lang::jint x,
            ::jxx::lang::jint y) = 0;

        virtual ::jxx::lang::jint getIconWidth() const = 0;
        virtual ::jxx::lang::jint getIconHeight() const = 0;
    };
}
