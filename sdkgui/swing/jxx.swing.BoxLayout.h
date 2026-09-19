#pragma once

#include "awt/jxx.awt.LayoutManager2.h"
#include "lang/jxx.lang.Object.h"

namespace jxx::swing
{
    class BoxLayout : public ::jxx::lang::ClassBase<BoxLayout,
        ::jxx::lang::Object, ::jxx::awt::LayoutManager2>
    {
    public:
        using JxxSuper = ::jxx::lang::Object;
        using Super = ::jxx::lang::ClassBase<BoxLayout, JxxSuper,
            ::jxx::awt::LayoutManager2>;

        static constexpr ::jxx::lang::jint X_AXIS = 0;
        static constexpr ::jxx::lang::jint Y_AXIS = 1;
        static constexpr ::jxx::lang::jint LINE_AXIS = 2;
        static constexpr ::jxx::lang::jint PAGE_AXIS = 3;

        BoxLayout(const ::jxx::Ptr<::jxx::awt::Container>& target,
            ::jxx::lang::jint axis);

        ::jxx::Ptr<::jxx::awt::Container> getTarget() const;
        ::jxx::lang::jint getAxis() const;

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
        ::jxx::lang::jbool horizontal() const;
        ::jxx::Ptr<::jxx::awt::Container> target_;
        ::jxx::lang::jint axis_;
    };
}
