#pragma once

#include <string>
#include <unordered_map>

#include "awt/jxx.awt.LayoutManager2.h"
#include "lang/jxx.lang.ClassInfo.h"
#include "lang/jxx.lang.Object.h"

namespace jxx::awt
{
    class CardLayout : public ::jxx::lang::ClassBase<CardLayout,
        ::jxx::lang::Object, LayoutManager2>
    {
    public:
        using JxxSuper = ::jxx::lang::Object;
        using Super = ::jxx::lang::ClassBase<CardLayout,
            JxxSuper, LayoutManager2>;

        CardLayout();
        CardLayout(::jxx::lang::jint hgap, ::jxx::lang::jint vgap);

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
        ::jxx::Ptr<Dimension> preferredLayoutSize(
            const ::jxx::Ptr<Container>& parent) override;
        ::jxx::Ptr<Dimension> minimumLayoutSize(
            const ::jxx::Ptr<Container>& parent) override;
        ::jxx::Ptr<Dimension> maximumLayoutSize(
            const ::jxx::Ptr<Container>& target) override;
        ::jxx::lang::jfloat getLayoutAlignmentX(
            const ::jxx::Ptr<Container>& parent) override;
        ::jxx::lang::jfloat getLayoutAlignmentY(
            const ::jxx::Ptr<Container>& parent) override;
        void invalidateLayout(const ::jxx::Ptr<Container>& target) override;
        void layoutContainer(const ::jxx::Ptr<Container>& parent) override;

        void first(const ::jxx::Ptr<Container>& parent);
        void next(const ::jxx::Ptr<Container>& parent);
        void previous(const ::jxx::Ptr<Container>& parent);
        void last(const ::jxx::Ptr<Container>& parent);
        void show(const ::jxx::Ptr<Container>& parent,
            const ::jxx::Ptr<::jxx::lang::String>& name);
        ::jxx::Ptr<::jxx::lang::String> toString() const override;

    private:
        ::jxx::lang::jint visibleIndex(const ::jxx::Ptr<Container>& parent) const;
        void showIndex(const ::jxx::Ptr<Container>& parent,
            ::jxx::lang::jint index);

        ::jxx::lang::jint hgap_;
        ::jxx::lang::jint vgap_;
        std::unordered_map<std::string, ::jxx::Ptr<Component>> namedCards_;
    };
}
