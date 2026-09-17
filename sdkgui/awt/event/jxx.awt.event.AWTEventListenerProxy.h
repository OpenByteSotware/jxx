#pragma once

#include "awt/event/jxx.awt.event.AWTEventListener.h"
#include "util/jxx.util.EventListenerProxy.h"

namespace jxx::awt::event
{
    class AWTEventListenerProxy :
        public ::jxx::lang::ClassBase<AWTEventListenerProxy,
            ::jxx::util::EventListenerProxy<AWTEventListener>,
            AWTEventListener>
    {
    public:
        using JxxSuper = ::jxx::util::EventListenerProxy<AWTEventListener>;
        using Super = ::jxx::lang::ClassBase<AWTEventListenerProxy,
            JxxSuper, AWTEventListener>;

        AWTEventListenerProxy(::jxx::lang::jlong eventMask,
            const ::jxx::Ptr<AWTEventListener>& listener);
        ~AWTEventListenerProxy() override = default;

        void eventDispatched(
            const ::jxx::Ptr<::jxx::awt::AWTEvent>& event) override;
        ::jxx::lang::jlong getEventMask() const;

    private:
        ::jxx::lang::jlong eventMask_;
    };
}
