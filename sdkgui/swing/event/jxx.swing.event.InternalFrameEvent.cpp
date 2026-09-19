#include "swing/event/jxx.swing.event.InternalFrameEvent.h"
#include "swing/jxx.swing.JInternalFrame.h"

namespace jxx::swing::event
{
    InternalFrameEvent::InternalFrameEvent(
        const ::jxx::Ptr<::jxx::swing::JInternalFrame>& source,
        ::jxx::lang::jint id)
        : Super(::jxx::CAST<::jxx::lang::Object>(source), id)
    {
    }

    ::jxx::Ptr<::jxx::swing::JInternalFrame>
    InternalFrameEvent::getInternalFrame() const
    {
        return ::jxx::CAST<::jxx::swing::JInternalFrame>(source);
    }
}
