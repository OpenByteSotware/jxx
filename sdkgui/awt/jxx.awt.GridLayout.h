#pragma once

#include "awt/jxx.awt.LayoutManager.h"
#include "lang/jxx.lang.ClassInfo.h"
#include "lang/jxx.lang.Object.h"

namespace jxx::awt
{
    class GridLayout :
        public ::jxx::lang::ClassBase<GridLayout, ::jxx::lang::Object, LayoutManager>
    {
    public:
        using JxxSuper = ::jxx::lang::Object;
        using Super = ::jxx::lang::ClassBase<GridLayout, JxxSuper, LayoutManager>;

        GridLayout();
        GridLayout(::jxx::lang::jint rows, ::jxx::lang::jint columns);
        GridLayout(::jxx::lang::jint rows, ::jxx::lang::jint columns,
            ::jxx::lang::jint hgap, ::jxx::lang::jint vgap);

        ::jxx::lang::jint getRows() const;
        void setRows(::jxx::lang::jint rows);
        ::jxx::lang::jint getColumns() const;
        void setColumns(::jxx::lang::jint columns);
        ::jxx::lang::jint getHgap() const;
        void setHgap(::jxx::lang::jint hgap);
        ::jxx::lang::jint getVgap() const;
        void setVgap(::jxx::lang::jint vgap);

        void addLayoutComponent(const ::jxx::Ptr<::jxx::lang::String>& name,
            const ::jxx::Ptr<Component>& component) override;
        void removeLayoutComponent(const ::jxx::Ptr<Component>& component) override;
        ::jxx::Ptr<Dimension> preferredLayoutSize(const ::jxx::Ptr<Container>& parent) override;
        ::jxx::Ptr<Dimension> minimumLayoutSize(const ::jxx::Ptr<Container>& parent) override;
        void layoutContainer(const ::jxx::Ptr<Container>& parent) override;

    private:
        ::jxx::lang::jint rows_;
        ::jxx::lang::jint columns_;
        ::jxx::lang::jint hgap_;
        ::jxx::lang::jint vgap_;
    };
}
