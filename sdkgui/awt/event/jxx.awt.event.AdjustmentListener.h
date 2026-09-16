#pragma once

#include "util/jxx.util.EventListener.h"

namespace jxx::awt::event
{
    class AdjustmentEvent;

    class AdjustmentListener :
        public ::jxx::lang::InterfaceBase<AdjustmentListener,
            ::jxx::util::EventListener>
    {
    public:
        ~AdjustmentListener() override = default;

        virtual void adjustmentValueChanged(
            const ::jxx::Ptr<AdjustmentEvent>& event) = 0;
    };
}
