#pragma once

#include "awt/event/jxx.awt.event.HierarchyBoundsListener.h"
#include "lang/jxx.lang.ClassInfo.h"
#include "lang/jxx.lang.Object.h"

namespace jxx::awt::event
{
    class HierarchyBoundsAdapter :
        public ::jxx::lang::ClassBase<HierarchyBoundsAdapter,
            ::jxx::lang::Object,
            HierarchyBoundsListener>
    {
    public:
        using JxxSuper = ::jxx::lang::Object;
        using Super = ::jxx::lang::ClassBase<HierarchyBoundsAdapter,
            JxxSuper,
            HierarchyBoundsListener>;

        HierarchyBoundsAdapter() = default;
        ~HierarchyBoundsAdapter() override = default;

        void ancestorMoved(
            const ::jxx::Ptr<HierarchyEvent>& event) override;
        void ancestorResized(
            const ::jxx::Ptr<HierarchyEvent>& event) override;
    };
}
