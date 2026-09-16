#pragma once

#include "awt/event/jxx.awt.event.ItemListener.h"
#include "lang/jxx.lang.ClassInfo.h"
#include "lang/jxx.lang.buildin_array.h"
#include "lang/jxx.lang.Object.h"

namespace jxx::awt
{
    class ItemSelectable : public ::jxx::lang::InterfaceBase<ItemSelectable>
    {
    public:
        ~ItemSelectable() override = default;

        virtual ::jxx::lang::ObjectArray getSelectedObjects() const = 0;

        virtual void addItemListener(
            const ::jxx::Ptr<::jxx::awt::event::ItemListener>& listener) = 0;

        virtual void removeItemListener(
            const ::jxx::Ptr<::jxx::awt::event::ItemListener>& listener) = 0;
    };
}
