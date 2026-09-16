#pragma once

#include "awt/jxx.awt.AWTEvent.h"

namespace jxx::awt::event
{
    class ItemEvent :
        public ::jxx::lang::ClassBase<ItemEvent, ::jxx::awt::AWTEvent>
    {
    public:
        using JxxSuper = ::jxx::awt::AWTEvent;
        using Super = ::jxx::lang::ClassBase<ItemEvent, JxxSuper>;

        static constexpr ::jxx::lang::jint ITEM_FIRST = 701;
        static constexpr ::jxx::lang::jint ITEM_STATE_CHANGED = 701;
        static constexpr ::jxx::lang::jint ITEM_LAST = 701;
        static constexpr ::jxx::lang::jint SELECTED = 1;
        static constexpr ::jxx::lang::jint DESELECTED = 2;

        ItemEvent(const ::jxx::Ptr<::jxx::lang::Object>& source,
            ::jxx::lang::jint id,
            const ::jxx::Ptr<::jxx::lang::Object>& item,
            ::jxx::lang::jint stateChange);

        ::jxx::Ptr<::jxx::lang::Object> getItemSelectable() const;
        ::jxx::Ptr<::jxx::lang::Object> getItem() const;
        ::jxx::lang::jint getStateChange() const;

    protected:
        ::jxx::Ptr<::jxx::lang::String> paramString() const override;

    private:
        ::jxx::Ptr<::jxx::lang::Object> item_;
        ::jxx::lang::jint stateChange_;
    };
}
