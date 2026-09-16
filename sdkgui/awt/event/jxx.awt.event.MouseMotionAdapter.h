#pragma once

#include "awt/event/jxx.awt.event.MouseMotionListener.h"
#include "lang/jxx.lang.ClassInfo.h"
#include "lang/jxx.lang.Object.h"

namespace jxx::awt::event
{
    class MouseMotionAdapter :
        public ::jxx::lang::ClassBase<MouseMotionAdapter,
            ::jxx::lang::Object,
            MouseMotionListener>
    {
    public:
        using JxxSuper = ::jxx::lang::Object;
        using Super = ::jxx::lang::ClassBase<MouseMotionAdapter,
            JxxSuper,
            MouseMotionListener>;

        MouseMotionAdapter() = default;
        ~MouseMotionAdapter() override = default;

        void mouseDragged(const ::jxx::Ptr<MouseEvent>& event) override;
        void mouseMoved(const ::jxx::Ptr<MouseEvent>& event) override;
    };
}
