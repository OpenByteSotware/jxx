#pragma once

#include "awt/jxx.awt.Container.h"
#include "awt/jxx.awt.Dimension.h"
#include "awt/jxx.awt.Point.h"
#include "awt/jxx.awt.ScrollPaneAdjustable.h"

namespace jxx::awt
{
    class ScrollPane : public ::jxx::lang::ClassBase<ScrollPane, Container>
    {
    public:
        using JxxSuper = Container;
        using Super = ::jxx::lang::ClassBase<ScrollPane, JxxSuper>;

        static constexpr ::jxx::lang::jint SCROLLBARS_AS_NEEDED = 0;
        static constexpr ::jxx::lang::jint SCROLLBARS_ALWAYS = 1;
        static constexpr ::jxx::lang::jint SCROLLBARS_NEVER = 2;

        ScrollPane();
        explicit ScrollPane(::jxx::lang::jint scrollbarDisplayPolicy);
        ~ScrollPane() override = default;

        ::jxx::lang::jint getScrollbarDisplayPolicy() const;
        ::jxx::Ptr<Adjustable> getHAdjustable() const;
        ::jxx::Ptr<Adjustable> getVAdjustable() const;
        ::jxx::Ptr<Point> getScrollPosition() const;
        void setScrollPosition(const ::jxx::Ptr<Point>& position);
        void setScrollPosition(::jxx::lang::jint x, ::jxx::lang::jint y);
        ::jxx::Ptr<Dimension> getViewportSize() const;
        ::jxx::lang::jint getHScrollbarHeight() const;
        ::jxx::lang::jint getVScrollbarWidth() const;
        ::jxx::lang::jbool isWheelScrollingEnabled() const;
        void setWheelScrollingEnabled(::jxx::lang::jbool enabled);

        ::jxx::Ptr<Component> add(
            const ::jxx::Ptr<Component>& component) override;
        ::jxx::Ptr<Component> add(
            const ::jxx::Ptr<Component>& component,
            ::jxx::lang::jint index) override;
        ::jxx::Ptr<Component> add(
            const ::jxx::Ptr<::jxx::lang::String>& name,
            const ::jxx::Ptr<Component>& component) override;
        void doLayout() override;

    protected:
        ::jxx::Ptr<::jxx::lang::String> paramString() const;

    private:
        void updateAdjustables();

        ::jxx::lang::jint scrollbarDisplayPolicy_;
        ::jxx::Ptr<ScrollPaneAdjustable> horizontal_;
        ::jxx::Ptr<ScrollPaneAdjustable> vertical_;
        ::jxx::lang::jbool wheelScrollingEnabled_;
    };
}
