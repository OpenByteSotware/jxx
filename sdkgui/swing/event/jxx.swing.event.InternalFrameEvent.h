#pragma once

#include "awt/jxx.awt.AWTEvent.h"

namespace jxx::swing { class JInternalFrame; }

namespace jxx::swing::event
{
    class InternalFrameEvent : public ::jxx::lang::ClassBase<
        InternalFrameEvent, ::jxx::awt::AWTEvent>
    {
    public:
        using JxxSuper = ::jxx::awt::AWTEvent;
        using Super = ::jxx::lang::ClassBase<InternalFrameEvent, JxxSuper>;

        static constexpr ::jxx::lang::jint INTERNAL_FRAME_FIRST = 25549;
        static constexpr ::jxx::lang::jint INTERNAL_FRAME_OPENED = 25549;
        static constexpr ::jxx::lang::jint INTERNAL_FRAME_CLOSING = 25550;
        static constexpr ::jxx::lang::jint INTERNAL_FRAME_CLOSED = 25551;
        static constexpr ::jxx::lang::jint INTERNAL_FRAME_ICONIFIED = 25552;
        static constexpr ::jxx::lang::jint INTERNAL_FRAME_DEICONIFIED = 25553;
        static constexpr ::jxx::lang::jint INTERNAL_FRAME_ACTIVATED = 25554;
        static constexpr ::jxx::lang::jint INTERNAL_FRAME_DEACTIVATED = 25555;
        static constexpr ::jxx::lang::jint INTERNAL_FRAME_LAST = 25555;

        InternalFrameEvent(const ::jxx::Ptr<::jxx::swing::JInternalFrame>& source,
            ::jxx::lang::jint id);
        ::jxx::Ptr<::jxx::swing::JInternalFrame> getInternalFrame() const;
    };
}
