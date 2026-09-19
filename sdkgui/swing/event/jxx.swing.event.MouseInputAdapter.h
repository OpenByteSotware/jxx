#pragma once

#include "lang/jxx.lang.Object.h"
#include "swing/event/jxx.swing.event.MouseInputListener.h"

namespace jxx::swing::event
{
    class MouseInputAdapter : public ::jxx::lang::ClassBase<
        MouseInputAdapter,
        ::jxx::lang::Object,
        MouseInputListener>
    {
    public:
        using JxxSuper = ::jxx::lang::Object;
        using Super = ::jxx::lang::ClassBase<
            MouseInputAdapter, JxxSuper, MouseInputListener>;

        ~MouseInputAdapter() override = default;
        void mouseClicked(
            const ::jxx::Ptr<::jxx::awt::event::MouseEvent>& event) override;
        void mousePressed(
            const ::jxx::Ptr<::jxx::awt::event::MouseEvent>& event) override;
        void mouseReleased(
            const ::jxx::Ptr<::jxx::awt::event::MouseEvent>& event) override;
        void mouseEntered(
            const ::jxx::Ptr<::jxx::awt::event::MouseEvent>& event) override;
        void mouseExited(
            const ::jxx::Ptr<::jxx::awt::event::MouseEvent>& event) override;
        void mouseDragged(
            const ::jxx::Ptr<::jxx::awt::event::MouseEvent>& event) override;
        void mouseMoved(
            const ::jxx::Ptr<::jxx::awt::event::MouseEvent>& event) override;
    };
}
