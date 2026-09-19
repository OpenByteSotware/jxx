#pragma once

#include "util/jxx.util.EventListener.h"

namespace jxx::swing::event
{
    class InternalFrameEvent;

    class InternalFrameListener : public ::jxx::lang::InterfaceBase<
        InternalFrameListener, ::jxx::util::EventListener>
    {
    public:
        ~InternalFrameListener() override = default;
        virtual void internalFrameOpened(
            const ::jxx::Ptr<InternalFrameEvent>& event) = 0;
        virtual void internalFrameClosing(
            const ::jxx::Ptr<InternalFrameEvent>& event) = 0;
        virtual void internalFrameClosed(
            const ::jxx::Ptr<InternalFrameEvent>& event) = 0;
        virtual void internalFrameIconified(
            const ::jxx::Ptr<InternalFrameEvent>& event) = 0;
        virtual void internalFrameDeiconified(
            const ::jxx::Ptr<InternalFrameEvent>& event) = 0;
        virtual void internalFrameActivated(
            const ::jxx::Ptr<InternalFrameEvent>& event) = 0;
        virtual void internalFrameDeactivated(
            const ::jxx::Ptr<InternalFrameEvent>& event) = 0;
    };
}
