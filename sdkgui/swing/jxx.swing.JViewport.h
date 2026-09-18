#pragma once
#include "awt/jxx.awt.Dimension.h"
#include "awt/jxx.awt.Point.h"
#include "swing/jxx.swing.JComponent.h"
namespace jxx::swing
{
    class JViewport : public ::jxx::lang::ClassBase<JViewport, JComponent>
    {
    public:
        using JxxSuper=JComponent; using Super=::jxx::lang::ClassBase<JViewport,JxxSuper>;
        static constexpr ::jxx::lang::jint BLIT_SCROLL_MODE=1,BACKINGSTORE_SCROLL_MODE=2,SIMPLE_SCROLL_MODE=0;
        JViewport();
        void setView(const ::jxx::Ptr<::jxx::awt::Component>& view);
        ::jxx::Ptr<::jxx::awt::Component> getView() const;
        void setViewPosition(const ::jxx::Ptr<::jxx::awt::Point>& point);
        ::jxx::Ptr<::jxx::awt::Point> getViewPosition() const;
        void setViewSize(const ::jxx::Ptr<::jxx::awt::Dimension>& size);
        ::jxx::Ptr<::jxx::awt::Dimension> getViewSize() const;
        void setScrollMode(::jxx::lang::jint mode); ::jxx::lang::jint getScrollMode() const;
    private:
        ::jxx::Ptr<::jxx::awt::Component> view_;
        ::jxx::Ptr<::jxx::awt::Point> position_;
        ::jxx::Ptr<::jxx::awt::Dimension> viewSize_;
        ::jxx::lang::jint scrollMode_=BLIT_SCROLL_MODE;
    };
}
