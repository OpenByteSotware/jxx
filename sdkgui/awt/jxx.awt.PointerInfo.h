#pragma once

#include "awt/jxx.awt.Point.h"
#include "lang/jxx.lang.ClassInfo.h"
#include "lang/jxx.lang.Object.h"

namespace jxx::awt
{
    class GraphicsDevice;

    class PointerInfo final :
        public ::jxx::lang::ClassBase<PointerInfo, ::jxx::lang::Object>
    {
    public:
        using JxxSuper = ::jxx::lang::Object;
        using Super = ::jxx::lang::ClassBase<PointerInfo, JxxSuper>;

        PointerInfo(const ::jxx::Ptr<GraphicsDevice>& device,
            const ::jxx::Ptr<Point>& location);

        ::jxx::Ptr<GraphicsDevice> getDevice() const;
        ::jxx::Ptr<Point> getLocation() const;

    private:
        ::jxx::Ptr<GraphicsDevice> device_;
        ::jxx::Ptr<Point> location_;
    };
}
