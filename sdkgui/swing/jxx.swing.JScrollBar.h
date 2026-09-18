#pragma once
#include "awt/jxx.awt.Adjustable.h"
#include "swing/jxx.swing.JComponent.h"
namespace jxx::swing
{
    class JScrollBar : public ::jxx::lang::ClassBase<JScrollBar, JComponent, ::jxx::awt::Adjustable>
    {
    public:
        using JxxSuper = JComponent;
        using Super = ::jxx::lang::ClassBase<JScrollBar, JxxSuper, ::jxx::awt::Adjustable>;
        JScrollBar();
        explicit JScrollBar(::jxx::lang::jint orientation);
        JScrollBar(::jxx::lang::jint orientation, ::jxx::lang::jint value, ::jxx::lang::jint extent, ::jxx::lang::jint minimum, ::jxx::lang::jint maximum);
        ::jxx::lang::jint getOrientation() const override;
        void setMinimum(::jxx::lang::jint value) override; ::jxx::lang::jint getMinimum() const override;
        void setMaximum(::jxx::lang::jint value) override; ::jxx::lang::jint getMaximum() const override;
        void setUnitIncrement(::jxx::lang::jint value) override; ::jxx::lang::jint getUnitIncrement() const override;
        void setBlockIncrement(::jxx::lang::jint value) override; ::jxx::lang::jint getBlockIncrement() const override;
        void setVisibleAmount(::jxx::lang::jint value) override; ::jxx::lang::jint getVisibleAmount() const override;
        void setValue(::jxx::lang::jint value) override; ::jxx::lang::jint getValue() const override;
        void addAdjustmentListener(const ::jxx::Ptr<::jxx::awt::event::AdjustmentListener>& listener) override;
        void removeAdjustmentListener(const ::jxx::Ptr<::jxx::awt::event::AdjustmentListener>& listener) override;
    private:
        ::jxx::lang::jint orientation_, value_, extent_, minimum_, maximum_, unitIncrement_=1, blockIncrement_=10;
        std::vector<::jxx::Ptr<::jxx::awt::event::AdjustmentListener>> listeners_;
    };
}
