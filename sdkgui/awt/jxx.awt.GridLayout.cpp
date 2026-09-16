#include "awt/jxx.awt.GridLayout.h"

#include <algorithm>
#include "awt/jxx.awt.Component.h"
#include "awt/jxx.awt.Container.h"
#include "awt/jxx.awt.Dimension.h"
#include "lang/jxx.lang.IllegalArgumentException.h"

namespace jxx::awt
{
    GridLayout::GridLayout() : GridLayout(1, 0, 0, 0) {}
    GridLayout::GridLayout(::jxx::lang::jint rows, ::jxx::lang::jint columns)
        : GridLayout(rows, columns, 0, 0) {}
    GridLayout::GridLayout(::jxx::lang::jint rows, ::jxx::lang::jint columns,
        ::jxx::lang::jint hgap, ::jxx::lang::jint vgap)
        : rows_(rows), columns_(columns), hgap_(hgap), vgap_(vgap)
    {
        if (rows == 0 && columns == 0)
            throw ::jxx::lang::IllegalArgumentException("rows and columns cannot both be zero");
    }
    ::jxx::lang::jint GridLayout::getRows() const { return rows_; }
    void GridLayout::setRows(::jxx::lang::jint rows) { if (rows == 0 && columns_ == 0) throw ::jxx::lang::IllegalArgumentException("rows and columns cannot both be zero"); rows_ = rows; }
    ::jxx::lang::jint GridLayout::getColumns() const { return columns_; }
    void GridLayout::setColumns(::jxx::lang::jint columns) { if (columns == 0 && rows_ == 0) throw ::jxx::lang::IllegalArgumentException("rows and columns cannot both be zero"); columns_ = columns; }
    ::jxx::lang::jint GridLayout::getHgap() const { return hgap_; }
    void GridLayout::setHgap(::jxx::lang::jint hgap) { hgap_ = hgap; }
    ::jxx::lang::jint GridLayout::getVgap() const { return vgap_; }
    void GridLayout::setVgap(::jxx::lang::jint vgap) { vgap_ = vgap; }
    void GridLayout::addLayoutComponent(const ::jxx::Ptr<::jxx::lang::String>&, const ::jxx::Ptr<Component>&) {}
    void GridLayout::removeLayoutComponent(const ::jxx::Ptr<Component>&) {}

    ::jxx::Ptr<Dimension> GridLayout::preferredLayoutSize(const ::jxx::Ptr<Container>& parent)
    {
        if (!parent || parent->getComponentCount() == 0) return ::jxx::NEW<Dimension>(0, 0);
        ::jxx::lang::jint maxWidth = 0, maxHeight = 0;
        for (::jxx::lang::jint i = 0; i < parent->getComponentCount(); ++i)
        {
            auto component = parent->getComponent(i);
            if (!component) continue;
            maxWidth = std::max(maxWidth, component->getWidth());
            maxHeight = std::max(maxHeight, component->getHeight());
        }
        const auto count = parent->getComponentCount();
        const auto rows = rows_ > 0 ? rows_ : (count + columns_ - 1) / columns_;
        const auto columns = rows_ > 0 ? (count + rows_ - 1) / rows_ : columns_;
        return ::jxx::NEW<Dimension>(columns * maxWidth + (columns - 1) * hgap_, rows * maxHeight + (rows - 1) * vgap_);
    }

    ::jxx::Ptr<Dimension> GridLayout::minimumLayoutSize(const ::jxx::Ptr<Container>& parent)
    {
        return preferredLayoutSize(parent);
    }

    void GridLayout::layoutContainer(const ::jxx::Ptr<Container>& parent)
    {
        if (!parent || parent->getComponentCount() == 0) return;
        const auto count = parent->getComponentCount();
        const auto rows = rows_ > 0 ? rows_ : (count + columns_ - 1) / columns_;
        const auto columns = rows_ > 0 ? (count + rows_ - 1) / rows_ : columns_;
        const auto width = (parent->getWidth() - (columns - 1) * hgap_) / columns;
        const auto height = (parent->getHeight() - (rows - 1) * vgap_) / rows;
        for (::jxx::lang::jint i = 0; i < count; ++i)
        {
            const auto row = i / columns;
            const auto column = i % columns;
            parent->getComponent(i)->setBounds(column * (width + hgap_), row * (height + vgap_), width, height);
        }
    }
}
