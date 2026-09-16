#pragma once

#include "awt/event/jxx.awt.event.AdjustmentListener.h"
#include "lang/jxx.lang.ClassInfo.h"
#include "lang/jxx_types.h"

namespace jxx::awt
{
    class Adjustable : public ::jxx::lang::InterfaceBase<Adjustable>
    {
    public:
        static constexpr ::jxx::lang::jint HORIZONTAL = 0;
        static constexpr ::jxx::lang::jint VERTICAL = 1;
        static constexpr ::jxx::lang::jint NO_ORIENTATION = 2;

        ~Adjustable() override = default;

        virtual ::jxx::lang::jint getOrientation() const = 0;
        virtual void setMinimum(::jxx::lang::jint minimum) = 0;
        virtual ::jxx::lang::jint getMinimum() const = 0;
        virtual void setMaximum(::jxx::lang::jint maximum) = 0;
        virtual ::jxx::lang::jint getMaximum() const = 0;
        virtual void setUnitIncrement(::jxx::lang::jint increment) = 0;
        virtual ::jxx::lang::jint getUnitIncrement() const = 0;
        virtual void setBlockIncrement(::jxx::lang::jint increment) = 0;
        virtual ::jxx::lang::jint getBlockIncrement() const = 0;
        virtual void setVisibleAmount(::jxx::lang::jint amount) = 0;
        virtual ::jxx::lang::jint getVisibleAmount() const = 0;
        virtual void setValue(::jxx::lang::jint value) = 0;
        virtual ::jxx::lang::jint getValue() const = 0;

        virtual void addAdjustmentListener(
            const ::jxx::Ptr<::jxx::awt::event::AdjustmentListener>& listener) = 0;

        virtual void removeAdjustmentListener(
            const ::jxx::Ptr<::jxx::awt::event::AdjustmentListener>& listener) = 0;
    };
}
