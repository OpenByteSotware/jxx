#pragma once

#include "util/jxx.util.EventListener.h"

namespace jxx::awt::event
{
    class ItemEvent;

    class ItemListener :
        public ::jxx::lang::InterfaceBase<ItemListener,
            ::jxx::util::EventListener>
    {
    public:
        ~ItemListener() override = default;
        virtual void itemStateChanged(
            const ::jxx::Ptr<ItemEvent>& event) = 0;
    };
}
