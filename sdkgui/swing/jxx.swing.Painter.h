#pragma once

#include "lang/jxx.lang.ClassInfo.h"

namespace jxx::awt
{
    class Graphics2D;
}

namespace jxx::swing
{
    template<typename T>
    class Painter : public ::jxx::lang::InterfaceBase<Painter<T>>
    {
    public:
        ~Painter() override = default;

        virtual void paint(
            const ::jxx::Ptr<::jxx::awt::Graphics2D>& graphics,
            const ::jxx::Ptr<T>& object,
            ::jxx::lang::jint width,
            ::jxx::lang::jint height) = 0;
    };
}
