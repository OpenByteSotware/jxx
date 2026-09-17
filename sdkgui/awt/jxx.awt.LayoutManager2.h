#pragma once

#include "awt/jxx.awt.LayoutManager.h"
#include "lang/jxx.lang.Object.h"

namespace jxx::awt
{
    class LayoutManager2 : public ::jxx::lang::InterfaceBase<
        LayoutManager2, LayoutManager>
    {
    public:
        ~LayoutManager2() override = default;

        virtual void addLayoutComponent(
            const ::jxx::Ptr<Component>& component,
            const ::jxx::Ptr<::jxx::lang::Object>& constraints) = 0;
        virtual ::jxx::Ptr<Dimension> maximumLayoutSize(
            const ::jxx::Ptr<Container>& target) = 0;
        virtual ::jxx::lang::jfloat getLayoutAlignmentX(
            const ::jxx::Ptr<Container>& target) = 0;
        virtual ::jxx::lang::jfloat getLayoutAlignmentY(
            const ::jxx::Ptr<Container>& target) = 0;
        virtual void invalidateLayout(
            const ::jxx::Ptr<Container>& target) = 0;
    };
}
