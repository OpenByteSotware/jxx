#pragma once

#include <unordered_map>

#include "awt/jxx.awt.Dimension.h"
#include "awt/jxx.awt.GridBagConstraints.h"
#include "awt/jxx.awt.LayoutManager2.h"
#include "awt/jxx.awt.Point.h"

namespace jxx::awt
{
    class GridBagLayout : public ::jxx::lang::ClassBase<GridBagLayout,
        ::jxx::lang::Object, LayoutManager2>
    {
    public:
        using JxxSuper = ::jxx::lang::Object;
        using Super = ::jxx::lang::ClassBase<GridBagLayout,
            JxxSuper, LayoutManager2>;

        GridBagLayout();
        void setConstraints(const ::jxx::Ptr<Component>& component,
            const ::jxx::Ptr<GridBagConstraints>& constraints);
        ::jxx::Ptr<GridBagConstraints> getConstraints(
            const ::jxx::Ptr<Component>& component) const;
        void addLayoutComponent(
            const ::jxx::Ptr<::jxx::lang::String>& name,
            const ::jxx::Ptr<Component>& component) override;
        void addLayoutComponent(const ::jxx::Ptr<Component>& component,
            const ::jxx::Ptr<::jxx::lang::Object>& constraints) override;
        void removeLayoutComponent(
            const ::jxx::Ptr<Component>& component) override;
        ::jxx::Ptr<Dimension> preferredLayoutSize(
            const ::jxx::Ptr<Container>& parent) override;
        ::jxx::Ptr<Dimension> minimumLayoutSize(
            const ::jxx::Ptr<Container>& parent) override;
        ::jxx::Ptr<Dimension> maximumLayoutSize(
            const ::jxx::Ptr<Container>& target) override;
        ::jxx::lang::jfloat getLayoutAlignmentX(
            const ::jxx::Ptr<Container>& target) override;
        ::jxx::lang::jfloat getLayoutAlignmentY(
            const ::jxx::Ptr<Container>& target) override;
        void invalidateLayout(const ::jxx::Ptr<Container>& target) override;
        void layoutContainer(const ::jxx::Ptr<Container>& parent) override;
        ::jxx::Ptr<Point> getLayoutOrigin() const;
        ::jxx::Ptr<Dimension> getLayoutDimensions() const;
        ::jxx::Ptr<Point> location(::jxx::lang::jint x,
            ::jxx::lang::jint y) const;
        ::jxx::Ptr<::jxx::lang::String> toString() const override;

    protected:
        ::jxx::Ptr<GridBagConstraints> lookupConstraints(
            const ::jxx::Ptr<Component>& component) const;

    private:
        struct Cell
        {
            ::jxx::lang::jint x;
            ::jxx::lang::jint y;
            ::jxx::lang::jint w;
            ::jxx::lang::jint h;
        };

        std::unordered_map<Component*, ::jxx::Ptr<GridBagConstraints>>
            constraints_;
        ::jxx::Ptr<GridBagConstraints> defaults_;
        ::jxx::lang::jint originX_ = 0;
        ::jxx::lang::jint originY_ = 0;
        ::jxx::lang::jint columns_ = 0;
        ::jxx::lang::jint rows_ = 0;
        ::jxx::lang::jint cellWidth_ = 0;
        ::jxx::lang::jint cellHeight_ = 0;
    };
}
