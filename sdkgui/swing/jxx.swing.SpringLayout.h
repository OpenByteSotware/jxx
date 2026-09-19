#pragma once

#include <unordered_map>

#include "awt/jxx.awt.LayoutManager2.h"
#include "lang/jxx.lang.Object.h"
#include "swing/jxx.swing.Spring.h"

namespace jxx::swing
{
    class SpringLayout : public ::jxx::lang::ClassBase<SpringLayout,
        ::jxx::lang::Object, ::jxx::awt::LayoutManager2>
    {
    public:
        using JxxSuper = ::jxx::lang::Object;
        using Super = ::jxx::lang::ClassBase<SpringLayout, JxxSuper,
            ::jxx::awt::LayoutManager2>;

        static constexpr const char* NORTH = "North";
        static constexpr const char* SOUTH = "South";
        static constexpr const char* EAST = "East";
        static constexpr const char* WEST = "West";
        static constexpr const char* HORIZONTAL_CENTER = "HorizontalCenter";
        static constexpr const char* VERTICAL_CENTER = "VerticalCenter";
        static constexpr const char* BASELINE = "Baseline";
        static constexpr const char* WIDTH = "Width";
        static constexpr const char* HEIGHT = "Height";

        class Constraints : public ::jxx::lang::ClassBase<Constraints,
            ::jxx::lang::Object>
        {
        public:
            using JxxSuper = ::jxx::lang::Object;
            using Super = ::jxx::lang::ClassBase<Constraints, JxxSuper>;
            Constraints();
            Constraints(const ::jxx::Ptr<Spring>& x,
                const ::jxx::Ptr<Spring>& y);
            void setX(const ::jxx::Ptr<Spring>& spring);
            ::jxx::Ptr<Spring> getX() const;
            void setY(const ::jxx::Ptr<Spring>& spring);
            ::jxx::Ptr<Spring> getY() const;
            void setWidth(const ::jxx::Ptr<Spring>& spring);
            ::jxx::Ptr<Spring> getWidth() const;
            void setHeight(const ::jxx::Ptr<Spring>& spring);
            ::jxx::Ptr<Spring> getHeight() const;
        private:
            ::jxx::Ptr<Spring> x_;
            ::jxx::Ptr<Spring> y_;
            ::jxx::Ptr<Spring> width_;
            ::jxx::Ptr<Spring> height_;
        };

        SpringLayout() = default;
        ::jxx::Ptr<Constraints> getConstraints(
            const ::jxx::Ptr<::jxx::awt::Component>& component);
        ::jxx::Ptr<Spring> getConstraint(
            const ::jxx::Ptr<::jxx::lang::String>& edge,
            const ::jxx::Ptr<::jxx::awt::Component>& component);
        void putConstraint(const ::jxx::Ptr<::jxx::lang::String>& edge1,
            const ::jxx::Ptr<::jxx::awt::Component>& component1,
            ::jxx::lang::jint padding,
            const ::jxx::Ptr<::jxx::lang::String>& edge2,
            const ::jxx::Ptr<::jxx::awt::Component>& component2);

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
        std::unordered_map<const ::jxx::awt::Component*,
            ::jxx::Ptr<Constraints>> constraints_;
    };
}
