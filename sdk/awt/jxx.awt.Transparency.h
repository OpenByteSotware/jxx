#pragma once

#include "lang/jxx.lang.ClassInfo.h"
#include "lang/jxx_types.h"

namespace jxx::awt
{
    class Transparency : public ::jxx::lang::InterfaceBase<Transparency>
    {
    public:
        static constexpr ::jxx::lang::jint OPAQUE = 1;
        static constexpr ::jxx::lang::jint BITMASK = 2;
        static constexpr ::jxx::lang::jint TRANSLUCENT = 3;

        ~Transparency() override = default;
        virtual ::jxx::lang::jint getTransparency() const = 0;
    };
}
