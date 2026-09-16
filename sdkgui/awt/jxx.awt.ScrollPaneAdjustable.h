#pragma once

#include <vector>

#include "awt/jxx.awt.Adjustable.h"
#include "lang/jxx.lang.ClassInfo.h"
#include "lang/jxx.lang.Object.h"
#include "lang/jxx.lang.String.h"

namespace jxx::awt::event
{
    class AdjustmentEvent;
    class AdjustmentListener;
}

namespace jxx::awt
{
    class ScrollPane;

    class ScrollPaneAdjustable :
        public ::jxx::lang::ClassBase<ScrollPaneAdjustable,
            ::jxx::lang::Object, Adjustable>
    {
    public:
        using JxxSuper = ::jxx::lang::Object;
        using Super = ::jxx::lang::ClassBase<ScrollPaneAdjustable,
            JxxSuper, Adjustable>;

        ~ScrollPaneAdjustable() override = default;

        ::jxx::lang::jint getOrientation() const override;
        void setMinimum(::jxx::lang::jint minimum) override;
        ::jxx::lang::jint getMinimum() const override;
        void setMaximum(::jxx::lang::jint maximum) override;
        ::jxx::lang::jint getMaximum() const override;
        void setUnitIncrement(::jxx::lang::jint increment) override;
        ::jxx::lang::jint getUnitIncrement() const override;
        void setBlockIncrement(::jxx::lang::jint increment) override;
        ::jxx::lang::jint getBlockIncrement() const override;
        void setVisibleAmount(::jxx::lang::jint amount) override;
        ::jxx::lang::jint getVisibleAmount() const override;
        void setValue(::jxx::lang::jint value) override;
        ::jxx::lang::jint getValue() const override;
        ::jxx::lang::jbool getValueIsAdjusting() const;
        void setValueIsAdjusting(::jxx::lang::jbool adjusting);

        void addAdjustmentListener(
            const ::jxx::Ptr<::jxx::awt::event::AdjustmentListener>& listener) override;
        void removeAdjustmentListener(
            const ::jxx::Ptr<::jxx::awt::event::AdjustmentListener>& listener) override;

        ::jxx::Ptr<::jxx::lang::String> paramString() const;
        ::jxx::Ptr<::jxx::lang::String> toString() const override;

    private:
        friend class ScrollPane;

        explicit ScrollPaneAdjustable(::jxx::lang::jint orientation);
        void setSpan(::jxx::lang::jint minimum,
            ::jxx::lang::jint maximum,
            ::jxx::lang::jint visibleAmount);
        void setValueInternal(::jxx::lang::jint value,
            ::jxx::lang::jint adjustmentType,
            ::jxx::lang::jbool adjusting,
            ::jxx::lang::jbool fireEvent);
        ::jxx::lang::jint clampValue(::jxx::lang::jint value) const;
        void fireAdjustmentEvent(::jxx::lang::jint adjustmentType);

        ::jxx::lang::jint orientation_;
        ::jxx::lang::jint value_;
        ::jxx::lang::jint minimum_;
        ::jxx::lang::jint maximum_;
        ::jxx::lang::jint visibleAmount_;
        ::jxx::lang::jint unitIncrement_;
        ::jxx::lang::jint blockIncrement_;
        ::jxx::lang::jbool valueIsAdjusting_;
        std::vector<::jxx::Ptr<::jxx::awt::event::AdjustmentListener>> listeners_;
    };
}
