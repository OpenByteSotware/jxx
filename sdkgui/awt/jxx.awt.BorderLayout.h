#pragma once

#include <string>
#include <unordered_map>

#include "awt/jxx.awt.LayoutManager2.h"
#include "lang/jxx.lang.ClassInfo.h"
#include "lang/jxx.lang.Object.h"

namespace jxx::awt
{
    class BorderLayout : public ::jxx::lang::ClassBase<BorderLayout,
        ::jxx::lang::Object, LayoutManager2>
    {
    public:
        using JxxSuper = ::jxx::lang::Object;
        using Super = ::jxx::lang::ClassBase<BorderLayout,
            JxxSuper, LayoutManager2>;

        static ::jxx::Ptr<::jxx::lang::String> NORTH;
        static ::jxx::Ptr<::jxx::lang::String> SOUTH;
        static ::jxx::Ptr<::jxx::lang::String> EAST;
        static ::jxx::Ptr<::jxx::lang::String> WEST;
        static ::jxx::Ptr<::jxx::lang::String> CENTER;
        static ::jxx::Ptr<::jxx::lang::String> BEFORE_FIRST_LINE;
        static ::jxx::Ptr<::jxx::lang::String> AFTER_LAST_LINE;
        static ::jxx::Ptr<::jxx::lang::String> BEFORE_LINE_BEGINS;
        static ::jxx::Ptr<::jxx::lang::String> AFTER_LINE_ENDS;
        static ::jxx::Ptr<::jxx::lang::String> PAGE_START;
        static ::jxx::Ptr<::jxx::lang::String> PAGE_END;
        static ::jxx::Ptr<::jxx::lang::String> LINE_START;
        static ::jxx::Ptr<::jxx::lang::String> LINE_END;

        BorderLayout();
        BorderLayout(::jxx::lang::jint hgap, ::jxx::lang::jint vgap);
        ::jxx::lang::jint getHgap() const;
        void setHgap(::jxx::lang::jint hgap);
        ::jxx::lang::jint getVgap() const;
        void setVgap(::jxx::lang::jint vgap);

        void addLayoutComponent(
            const ::jxx::Ptr<::jxx::lang::String>& name,
            const ::jxx::Ptr<Component>& component) override;
        void addLayoutComponent(const ::jxx::Ptr<Component>& component,
            const ::jxx::Ptr<::jxx::lang::Object>& constraints) override;
        void removeLayoutComponent(
            const ::jxx::Ptr<Component>& component) override;
        ::jxx::Ptr<Component> getLayoutComponent(
            const ::jxx::Ptr<::jxx::lang::Object>& constraints) const;
        ::jxx::Ptr<Component> getLayoutComponent(
            const ::jxx::Ptr<Container>& target,
            const ::jxx::Ptr<::jxx::lang::Object>& constraints) const;
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
        ::jxx::Ptr<::jxx::lang::String> toString() const override;

    private:
        ::jxx::Ptr<Component> componentFor(const char* key) const;
        static std::string constraintKey(
            const ::jxx::Ptr<::jxx::lang::Object>& constraints);
        ::jxx::lang::jint hgap_;
        ::jxx::lang::jint vgap_;
        std::unordered_map<std::string, ::jxx::Ptr<Component>> components_;
    };
}
