#include "awt/event/jxx.awt.event.AWTEventListenerProxy.h"

namespace jxx::awt::event
{
    AWTEventListenerProxy::AWTEventListenerProxy(
        ::jxx::lang::jlong eventMask,
        const ::jxx::Ptr<AWTEventListener>& listener)
        : Super(listener), eventMask_(eventMask)
    {
    }

    void AWTEventListenerProxy::eventDispatched(
        const ::jxx::Ptr<::jxx::awt::AWTEvent>& event)
    {
        const auto listener = getListener();
        if (listener != nullptr) listener->eventDispatched(event);
    }

    ::jxx::lang::jlong AWTEventListenerProxy::getEventMask() const
    {
        return eventMask_;
    }
}
