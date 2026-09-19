#pragma once

#include "awt/jxx.awt.LayoutManager2.h"
#include "lang/jxx.lang.Object.h"

namespace jxx::swing
{
    class OverlayLayout : public ::jxx::lang::ClassBase<OverlayLayout,
        ::jxx::lang::Object, ::jxx::awt::LayoutManager2>
    {
    public:
        using JxxSuper = ::jxx::lang::Object;
        using Super = ::jxx::lang::ClassBase<OverlayLayout, JxxSuper,
            ::jxx::awt::LayoutManager2>;
        explicit OverlayLayout(
            const ::jxx::Ptr<::jxx::awt::Container>& target);
        ::jxx::Ptr<::jxx::awt::Container> getTarget() const;
        void addLayoutComponent(const ::jxx::Ptr<::jxx::lang::String>& name,
            const ::jxx::Ptr<::jxx::awt::Component>& component) override;
        void addLayoutComponent(const ::jxx::Ptr<::jxx::awt::Component>& component,
            const ::jxx::Ptr<::jxx::lang::Object>& constraints) override;
        void removeLayoutComponent(
            const ::jxx::Ptr<::jxx::awt::Component>& component) override;
        ::jxx::Ptr<::jxx::awt::Dimension> preferredLayoutSize(
            const ::jxx::Ptr<::jxx::awt::Container>& target) override;
        ::jxx::Ptr<::jxx::awt::Dimension> minimumLayoutSize(
            const ::jxx::Ptr<::jxx::awt::Container>& target) override;
        ::jxx::Ptr<::jxx::awt::Dimension> maximumLayoutSize(
            const ::jxx::Ptr<::jxx::awt::Container>& target) override;
        ::jxx::lang::jfloat getLayoutAlignmentX(
            const ::jxx::Ptr<::jxx::awt::Container>& target) override;
        ::jxx::lang::jfloat getLayoutAlignmentY(
            const ::jxx::Ptr<::jxx::awt::Container>& target) override;
        void invalidateLayout(
            const ::jxx::Ptr<::jxx::awt::Container>& target) override;
        void layoutContainer(
            const ::jxx::Ptr<::jxx::awt::Container>& target) override;
    private:
        ::jxx::Ptr<::jxx::awt::Container> target_;
    };
}
