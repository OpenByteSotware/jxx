#pragma once

#include "util/jxx.util.EventListener.h"

namespace jxx::awt::event
{
    class ContainerEvent;

    class ContainerListener :
        public ::jxx::lang::InterfaceBase<ContainerListener,
            ::jxx::util::EventListener>
    {
    public:
        ~ContainerListener() override = default;

        virtual void componentAdded(
            const ::jxx::Ptr<ContainerEvent>& event) = 0;
        virtual void componentRemoved(
            const ::jxx::Ptr<ContainerEvent>& event) = 0;
    };
}
