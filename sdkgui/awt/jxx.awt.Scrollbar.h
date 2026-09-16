#pragma once

#include <vector>

#include "awt/jxx.awt.Adjustable.h"
#include "awt/jxx.awt.Component.h"

namespace jxx::awt::event
{
    class AdjustmentEvent;
    class AdjustmentListener;
}

namespace jxx::awt
{
    class Scrollbar :
        public ::jxx::lang::ClassBase<Scrollbar, Component, Adjustable>
    {
    public:
        using JxxSuper = Component;
        using Super = ::jxx::lang::ClassBase<Scrollbar,
            JxxSuper, Adjustable>;

        static constexpr ::jxx::lang::jint HORIZONTAL = Adjustable::HORIZONTAL;
        static constexpr ::jxx::lang::jint VERTICAL = Adjustable::VERTICAL;

        Scrollbar();
        explicit Scrollbar(::jxx::lang::jint orientation);
        Scrollbar(::jxx::lang::jint orientation,
            ::jxx::lang::jint value,
            ::jxx::lang::jint visibleAmount,
            ::jxx::lang::jint minimum,
            ::jxx::lang::jint maximum);
        ~Scrollbar() override = default;

        ::jxx::lang::jint getOrientation() const override;
        void setValues(::jxx::lang::jint value,
            ::jxx::lang::jint visibleAmount,
            ::jxx::lang::jint minimum,
            ::jxx::lang::jint maximum);
        ::jxx::lang::jint getValue() const override;
        void setValue(::jxx::lang::jint value) override;
        ::jxx::lang::jint getMinimum() const override;
        void setMinimum(::jxx::lang::jint minimum) override;
        ::jxx::lang::jint getMaximum() const override;
        void setMaximum(::jxx::lang::jint maximum) override;
        ::jxx::lang::jint getVisibleAmount() const override;
        void setVisibleAmount(::jxx::lang::jint amount) override;
        ::jxx::lang::jint getUnitIncrement() const override;
        void setUnitIncrement(::jxx::lang::jint increment) override;
        ::jxx::lang::jint getLineIncrement() const;
        void setLineIncrement(::jxx::lang::jint increment);
        ::jxx::lang::jint getBlockIncrement() const override;
        void setBlockIncrement(::jxx::lang::jint increment) override;
        ::jxx::lang::jint getPageIncrement() const;
        void setPageIncrement(::jxx::lang::jint increment);
        ::jxx::lang::jbool getValueIsAdjusting() const;
        void setValueIsAdjusting(::jxx::lang::jbool adjusting);

        void addAdjustmentListener(
            const ::jxx::Ptr<::jxx::awt::event::AdjustmentListener>& listener) override;
        void removeAdjustmentListener(
            const ::jxx::Ptr<::jxx::awt::event::AdjustmentListener>& listener) override;

    protected:
        ::jxx::Ptr<::jxx::lang::String> paramString() const;
        virtual void processAdjustmentEvent(
            const ::jxx::Ptr<::jxx::awt::event::AdjustmentEvent>& event);

    private:
        void setValueFromNative(::jxx::lang::jint value,
            ::jxx::lang::jint adjustmentType,
            ::jxx::lang::jbool adjusting);
        void normalizeValues(::jxx::lang::jint value,
            ::jxx::lang::jint visibleAmount,
            ::jxx::lang::jint minimum,
            ::jxx::lang::jint maximum);

        ::jxx::lang::jint orientation_;
        ::jxx::lang::jint value_;
        ::jxx::lang::jint visibleAmount_;
        ::jxx::lang::jint minimum_;
        ::jxx::lang::jint maximum_;
        ::jxx::lang::jint unitIncrement_;
        ::jxx::lang::jint blockIncrement_;
        ::jxx::lang::jbool valueIsAdjusting_;
        std::vector<::jxx::Ptr<::jxx::awt::event::AdjustmentListener>> listeners_;
    };
}
