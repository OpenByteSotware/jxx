#pragma once

#include "awt/event/jxx.awt.event.ComponentListener.h"
#include "lang/jxx.lang.ClassInfo.h"
#include "lang/jxx.lang.Object.h"

namespace jxx::awt::event
{
    class ComponentAdapter :
        public ::jxx::lang::ClassBase<ComponentAdapter,
            ::jxx::lang::Object,
            ComponentListener>
    {
    public:
        using JxxSuper = ::jxx::lang::Object;
        using Super = ::jxx::lang::ClassBase<ComponentAdapter,
            JxxSuper,
            ComponentListener>;

        ComponentAdapter() = default;
        ~ComponentAdapter() override = default;

        void componentResized(
            const ::jxx::Ptr<ComponentEvent>& event) override;
        void componentMoved(
            const ::jxx::Ptr<ComponentEvent>& event) override;
        void componentShown(
            const ::jxx::Ptr<ComponentEvent>& event) override;
        void componentHidden(
            const ::jxx::Ptr<ComponentEvent>& event) override;
    };
}
