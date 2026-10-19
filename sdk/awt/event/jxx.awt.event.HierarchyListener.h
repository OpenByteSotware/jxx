#pragma once

#include "util/jxx.util.EventListener.h"

namespace jxx::awt::event
{
    class HierarchyEvent;

    class HierarchyListener :
        public ::jxx::lang::InterfaceBase<HierarchyListener,
            ::jxx::util::EventListener>
    {
    public:
        ~HierarchyListener() override = default;
        virtual void hierarchyChanged(
            const ::jxx::Ptr<HierarchyEvent>& event) = 0;
    };
}
