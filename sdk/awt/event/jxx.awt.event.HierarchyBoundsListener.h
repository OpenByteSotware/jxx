#pragma once

#include "util/jxx.util.EventListener.h"

namespace jxx::awt::event
{
    class HierarchyEvent;

    class HierarchyBoundsListener :
        public ::jxx::lang::InterfaceBase<HierarchyBoundsListener,
            ::jxx::util::EventListener>
    {
    public:
        ~HierarchyBoundsListener() override = default;
        virtual void ancestorMoved(
            const ::jxx::Ptr<HierarchyEvent>& event) = 0;
        virtual void ancestorResized(
            const ::jxx::Ptr<HierarchyEvent>& event) = 0;
    };
}
