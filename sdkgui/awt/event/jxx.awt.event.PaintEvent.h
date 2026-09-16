#pragma once

#include "awt/event/jxx.awt.event.ComponentEvent.h"
#include "awt/jxx.awt.Rectangle.h"

namespace jxx::awt::event
{
    class PaintEvent :
        public ::jxx::lang::ClassBase<PaintEvent, ComponentEvent>
    {
    public:
        using JxxSuper = ComponentEvent;
        using Super = ::jxx::lang::ClassBase<PaintEvent, JxxSuper>;

        static constexpr ::jxx::lang::jint PAINT_FIRST = 800;
        static constexpr ::jxx::lang::jint PAINT = 800;
        static constexpr ::jxx::lang::jint UPDATE = 801;
        static constexpr ::jxx::lang::jint PAINT_LAST = 801;

        PaintEvent(
            const ::jxx::Ptr<::jxx::awt::Component>& source,
            ::jxx::lang::jint id,
            const ::jxx::Ptr<::jxx::awt::Rectangle>& updateRect);

        ::jxx::Ptr<::jxx::awt::Rectangle> getUpdateRect() const;
        void setUpdateRect(
            const ::jxx::Ptr<::jxx::awt::Rectangle>& updateRect);

    protected:
        ::jxx::Ptr<::jxx::lang::String> paramString() const override;

    private:
        ::jxx::Ptr<::jxx::awt::Rectangle> updateRect_;
    };
}
