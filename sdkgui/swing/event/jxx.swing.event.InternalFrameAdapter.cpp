#include "swing/event/jxx.swing.event.InternalFrameAdapter.h"

namespace jxx::swing::event
{
    void InternalFrameAdapter::internalFrameOpened(
        const ::jxx::Ptr<InternalFrameEvent>&) {}
    void InternalFrameAdapter::internalFrameClosing(
        const ::jxx::Ptr<InternalFrameEvent>&) {}
    void InternalFrameAdapter::internalFrameClosed(
        const ::jxx::Ptr<InternalFrameEvent>&) {}
    void InternalFrameAdapter::internalFrameIconified(
        const ::jxx::Ptr<InternalFrameEvent>&) {}
    void InternalFrameAdapter::internalFrameDeiconified(
        const ::jxx::Ptr<InternalFrameEvent>&) {}
    void InternalFrameAdapter::internalFrameActivated(
        const ::jxx::Ptr<InternalFrameEvent>&) {}
    void InternalFrameAdapter::internalFrameDeactivated(
        const ::jxx::Ptr<InternalFrameEvent>&) {}
}
