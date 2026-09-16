#pragma once

#include <vector>

#include "awt/jxx.awt.MenuItem.h"

namespace jxx::awt::event
{
    class ItemEvent;
    class ItemListener;
}

namespace jxx::gui::internal
{
    class WxFramePeer;
}

namespace jxx::awt
{
    class CheckboxMenuItem :
        public ::jxx::lang::ClassBase<CheckboxMenuItem, MenuItem>
    {
    public:
        using JxxSuper = MenuItem;
        using Super = ::jxx::lang::ClassBase<CheckboxMenuItem, JxxSuper>;

        CheckboxMenuItem();
        explicit CheckboxMenuItem(
            const ::jxx::Ptr<::jxx::lang::String>& label);
        CheckboxMenuItem(
            const ::jxx::Ptr<::jxx::lang::String>& label,
            ::jxx::lang::jbool state);

        ::jxx::lang::jbool getState() const;
        void setState(::jxx::lang::jbool state);
        void addItemListener(
            const ::jxx::Ptr<::jxx::awt::event::ItemListener>& listener);
        void removeItemListener(
            const ::jxx::Ptr<::jxx::awt::event::ItemListener>& listener);

    protected:
        virtual void processItemEvent(
            const ::jxx::Ptr<::jxx::awt::event::ItemEvent>& event);

    private:
        friend class ::jxx::gui::internal::WxFramePeer;
        void setStateFromNative(::jxx::lang::jbool state);

        ::jxx::lang::jbool state_;
        std::vector<::jxx::Ptr<::jxx::awt::event::ItemListener>> listeners_;
    };
}
