#pragma once

#include "awt/event/jxx.awt.event.ContainerListener.h"
#include "lang/jxx.lang.ClassInfo.h"
#include "lang/jxx.lang.Object.h"

namespace jxx::awt::event
{
    class ContainerAdapter :
        public ::jxx::lang::ClassBase<ContainerAdapter,
            ::jxx::lang::Object,
            ContainerListener>
    {
    public:
        using JxxSuper = ::jxx::lang::Object;
        using Super = ::jxx::lang::ClassBase<ContainerAdapter,
            JxxSuper,
            ContainerListener>;

        ContainerAdapter() = default;
        ~ContainerAdapter() override = default;

        void componentAdded(
            const ::jxx::Ptr<ContainerEvent>& event) override;
        void componentRemoved(
            const ::jxx::Ptr<ContainerEvent>& event) override;
    };
}
