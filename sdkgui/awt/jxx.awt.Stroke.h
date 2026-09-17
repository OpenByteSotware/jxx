#pragma once

#include "lang/jxx.lang.ClassInfo.h"

namespace jxx::awt
{
    class Shape;

    class Stroke : public ::jxx::lang::InterfaceBase<Stroke>
    {
    public:
        ~Stroke() override = default;
        virtual ::jxx::Ptr<Shape> createStrokedShape(
            const ::jxx::Ptr<Shape>& shape) const = 0;
    };
}
