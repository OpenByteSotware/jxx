#pragma once

#include "util/jxx.util.EventListener.h"

namespace jxx::awt::event
{
    class ComponentEvent;

    class ComponentListener :
        public ::jxx::lang::InterfaceBase<ComponentListener,
            ::jxx::util::EventListener>
    {
    public:
        ~ComponentListener() override = default;

        virtual void componentResized(
            const ::jxx::Ptr<ComponentEvent>& event) = 0;
        virtual void componentMoved(
            const ::jxx::Ptr<ComponentEvent>& event) = 0;
        virtual void componentShown(
            const ::jxx::Ptr<ComponentEvent>& event) = 0;
        virtual void componentHidden(
            const ::jxx::Ptr<ComponentEvent>& event) = 0;
    };
}
