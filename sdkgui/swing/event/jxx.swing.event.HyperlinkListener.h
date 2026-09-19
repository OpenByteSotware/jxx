#pragma once

#include "util/jxx.util.EventListener.h"

namespace jxx::swing::event
{
    class HyperlinkEvent;
    class HyperlinkListener : public ::jxx::lang::InterfaceBase<
        HyperlinkListener, ::jxx::util::EventListener>
    {
    public:
        ~HyperlinkListener() override = default;
        virtual void hyperlinkUpdate(
            const ::jxx::Ptr<HyperlinkEvent>& event) = 0;
    };
}
