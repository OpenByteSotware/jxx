#pragma once

#include "lang/jxx.lang.Object.h"
#include "swing/event/jxx.swing.event.InternalFrameListener.h"

namespace jxx::swing::event
{
    class InternalFrameAdapter : public ::jxx::lang::ClassBase<
        InternalFrameAdapter, ::jxx::lang::Object, InternalFrameListener>
    {
    public:
        using JxxSuper = ::jxx::lang::Object;
        using Super = ::jxx::lang::ClassBase<InternalFrameAdapter,
            JxxSuper, InternalFrameListener>;
        ~InternalFrameAdapter() override = default;
        void internalFrameOpened(
            const ::jxx::Ptr<InternalFrameEvent>& event) override;
        void internalFrameClosing(
            const ::jxx::Ptr<InternalFrameEvent>& event) override;
        void internalFrameClosed(
            const ::jxx::Ptr<InternalFrameEvent>& event) override;
        void internalFrameIconified(
            const ::jxx::Ptr<InternalFrameEvent>& event) override;
        void internalFrameDeiconified(
            const ::jxx::Ptr<InternalFrameEvent>& event) override;
        void internalFrameActivated(
            const ::jxx::Ptr<InternalFrameEvent>& event) override;
        void internalFrameDeactivated(
            const ::jxx::Ptr<InternalFrameEvent>& event) override;
    };
}
