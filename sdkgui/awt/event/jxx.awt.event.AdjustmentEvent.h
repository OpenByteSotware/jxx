#pragma once

#include "awt/jxx.awt.AWTEvent.h"
#include "awt/jxx.awt.Adjustable.h"

namespace jxx::awt::event
{
    class AdjustmentEvent :
        public ::jxx::lang::ClassBase<AdjustmentEvent, ::jxx::awt::AWTEvent>
    {
    public:
        using JxxSuper = ::jxx::awt::AWTEvent;
        using Super = ::jxx::lang::ClassBase<AdjustmentEvent, JxxSuper>;

        static constexpr ::jxx::lang::jint ADJUSTMENT_FIRST = 601;
        static constexpr ::jxx::lang::jint ADJUSTMENT_VALUE_CHANGED = 601;
        static constexpr ::jxx::lang::jint ADJUSTMENT_LAST = 601;

        static constexpr ::jxx::lang::jint UNIT_INCREMENT = 1;
        static constexpr ::jxx::lang::jint UNIT_DECREMENT = 2;
        static constexpr ::jxx::lang::jint BLOCK_DECREMENT = 3;
        static constexpr ::jxx::lang::jint BLOCK_INCREMENT = 4;
        static constexpr ::jxx::lang::jint TRACK = 5;

        AdjustmentEvent(
            const ::jxx::Ptr<::jxx::awt::Adjustable>& source,
            ::jxx::lang::jint id,
            ::jxx::lang::jint type,
            ::jxx::lang::jint value);

        AdjustmentEvent(
            const ::jxx::Ptr<::jxx::awt::Adjustable>& source,
            ::jxx::lang::jint id,
            ::jxx::lang::jint type,
            ::jxx::lang::jint value,
            ::jxx::lang::jbool valueIsAdjusting);

        ::jxx::Ptr<::jxx::awt::Adjustable> getAdjustable() const;
        ::jxx::lang::jint getAdjustmentType() const;
        ::jxx::lang::jint getValue() const;
        ::jxx::lang::jbool getValueIsAdjusting() const;

    protected:
        ::jxx::Ptr<::jxx::lang::String> paramString() const override;

    private:
        ::jxx::lang::jint adjustmentType_;
        ::jxx::lang::jint value_;
        ::jxx::lang::jbool valueIsAdjusting_;
    };
}
