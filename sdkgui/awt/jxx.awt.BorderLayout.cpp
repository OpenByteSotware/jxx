#include "awt/jxx.awt.BorderLayout.h"

#include <algorithm>
#include <limits>
#include <sstream>

#include "awt/jxx.awt.Component.h"
#include "awt/jxx.awt.Container.h"
#include "awt/jxx.awt.Dimension.h"
#include "lang/jxx.lang.IllegalArgumentException.h"
#include "lang/jxx.lang.String.h"

namespace jxx::awt
{
    namespace
    {
        ::jxx::Ptr<::jxx::lang::String> text(const char* value)
        {
            return ::jxx::NEW<::jxx::lang::String>(value);
        }
    }

    ::jxx::Ptr<::jxx::lang::String> BorderLayout::NORTH = text("North");
    ::jxx::Ptr<::jxx::lang::String> BorderLayout::SOUTH = text("South");
    ::jxx::Ptr<::jxx::lang::String> BorderLayout::EAST = text("East");
    ::jxx::Ptr<::jxx::lang::String> BorderLayout::WEST = text("West");
    ::jxx::Ptr<::jxx::lang::String> BorderLayout::CENTER = text("Center");
    ::jxx::Ptr<::jxx::lang::String> BorderLayout::BEFORE_FIRST_LINE = text("First");
    ::jxx::Ptr<::jxx::lang::String> BorderLayout::AFTER_LAST_LINE = text("Last");
    ::jxx::Ptr<::jxx::lang::String> BorderLayout::BEFORE_LINE_BEGINS = text("Before");
    ::jxx::Ptr<::jxx::lang::String> BorderLayout::AFTER_LINE_ENDS = text("After");
    ::jxx::Ptr<::jxx::lang::String> BorderLayout::PAGE_START = BorderLayout::BEFORE_FIRST_LINE;
    ::jxx::Ptr<::jxx::lang::String> BorderLayout::PAGE_END = BorderLayout::AFTER_LAST_LINE;
    ::jxx::Ptr<::jxx::lang::String> BorderLayout::LINE_START = BorderLayout::BEFORE_LINE_BEGINS;
    ::jxx::Ptr<::jxx::lang::String> BorderLayout::LINE_END = BorderLayout::AFTER_LINE_ENDS;

    BorderLayout::BorderLayout() : BorderLayout(0, 0) {}
    BorderLayout::BorderLayout(::jxx::lang::jint hgap,
        ::jxx::lang::jint vgap) : hgap_(hgap), vgap_(vgap) {}
    ::jxx::lang::jint BorderLayout::getHgap() const { return hgap_; }
    void BorderLayout::setHgap(::jxx::lang::jint value) { hgap_ = value; }
    ::jxx::lang::jint BorderLayout::getVgap() const { return vgap_; }
    void BorderLayout::setVgap(::jxx::lang::jint value) { vgap_ = value; }

    std::string BorderLayout::constraintKey(
        const ::jxx::Ptr<::jxx::lang::Object>& constraints)
    {
        if (constraints == nullptr) return "Center";
        const auto value = ::jxx::CAST<::jxx::lang::String>(constraints);
        if (value == nullptr)
            throw ::jxx::lang::IllegalArgumentException("constraints");
        const auto key = value->utf8();
        if (key != "North" && key != "South" && key != "East" &&
            key != "West" && key != "Center" && key != "First" &&
            key != "Last" && key != "Before" && key != "After")
            throw ::jxx::lang::IllegalArgumentException("constraints");
        return key;
    }

    void BorderLayout::addLayoutComponent(
        const ::jxx::Ptr<::jxx::lang::String>& name,
        const ::jxx::Ptr<Component>& component)
    {
        addLayoutComponent(component,
            ::jxx::CAST<::jxx::lang::Object>(name));
    }

    void BorderLayout::addLayoutComponent(
        const ::jxx::Ptr<Component>& component,
        const ::jxx::Ptr<::jxx::lang::Object>& constraints)
    {
        components_[constraintKey(constraints)] = component;
    }

    void BorderLayout::removeLayoutComponent(
        const ::jxx::Ptr<Component>& component)
    {
        for (auto iterator = components_.begin(); iterator != components_.end();)
            if (iterator->second == component) iterator = components_.erase(iterator);
            else ++iterator;
    }

    ::jxx::Ptr<Component> BorderLayout::componentFor(const char* key) const
    {
        const auto iterator = components_.find(key);
        return iterator == components_.end() ? nullptr : iterator->second;
    }

    ::jxx::Ptr<Component> BorderLayout::getLayoutComponent(
        const ::jxx::Ptr<::jxx::lang::Object>& constraints) const
    {
        return componentFor(constraintKey(constraints).c_str());
    }

    ::jxx::Ptr<Component> BorderLayout::getLayoutComponent(
        const ::jxx::Ptr<Container>& target,
        const ::jxx::Ptr<::jxx::lang::Object>& constraints) const
    {
        const auto key = constraintKey(constraints);
        if (key == "Before")
            return componentFor(target != nullptr &&
                !target->getComponentOrientation()->isLeftToRight()
                ? "After" : "Before");
        if (key == "After")
            return componentFor(target != nullptr &&
                !target->getComponentOrientation()->isLeftToRight()
                ? "Before" : "After");
        return componentFor(key.c_str());
    }

    ::jxx::Ptr<Dimension> BorderLayout::preferredLayoutSize(
        const ::jxx::Ptr<Container>&)
    {
        auto north = componentFor("First"); if (!north) north = componentFor("North");
        auto south = componentFor("Last"); if (!south) south = componentFor("South");
        auto west = componentFor("Before"); if (!west) west = componentFor("West");
        auto east = componentFor("After"); if (!east) east = componentFor("East");
        auto center = componentFor("Center");
        const auto middleHeight = std::max({west ? west->getHeight() : 0,
            east ? east->getHeight() : 0, center ? center->getHeight() : 0});
        const auto middleWidth = (west ? west->getWidth() + hgap_ : 0) +
            (center ? center->getWidth() : 0) +
            (east ? hgap_ + east->getWidth() : 0);
        return ::jxx::NEW<Dimension>(std::max({middleWidth,
            north ? north->getWidth() : 0, south ? south->getWidth() : 0}),
            (north ? north->getHeight() + vgap_ : 0) + middleHeight +
            (south ? vgap_ + south->getHeight() : 0));
    }

    ::jxx::Ptr<Dimension> BorderLayout::minimumLayoutSize(
        const ::jxx::Ptr<Container>& parent)
    {
        return preferredLayoutSize(parent);
    }

    ::jxx::Ptr<Dimension> BorderLayout::maximumLayoutSize(
        const ::jxx::Ptr<Container>&)
    {
        const auto maximum = std::numeric_limits<::jxx::lang::jint>::max();
        return ::jxx::NEW<Dimension>(maximum, maximum);
    }

    ::jxx::lang::jfloat BorderLayout::getLayoutAlignmentX(
        const ::jxx::Ptr<Container>&) { return 0.5f; }
    ::jxx::lang::jfloat BorderLayout::getLayoutAlignmentY(
        const ::jxx::Ptr<Container>&) { return 0.5f; }
    void BorderLayout::invalidateLayout(const ::jxx::Ptr<Container>&) {}

    void BorderLayout::layoutContainer(const ::jxx::Ptr<Container>& parent)
    {
        if (!parent) return;
        auto top = 0, left = 0, right = parent->getWidth(), bottom = parent->getHeight();
        auto north = componentFor("First"); if (!north) north = componentFor("North");
        auto south = componentFor("Last"); if (!south) south = componentFor("South");
        const auto leftToRight = parent->getComponentOrientation()->isLeftToRight();
        auto west = componentFor(leftToRight ? "Before" : "After");
        if (!west) west = componentFor("West");
        auto east = componentFor(leftToRight ? "After" : "Before");
        if (!east) east = componentFor("East");
        auto center = componentFor("Center");
        if (north && north->isVisible()) { north->setBounds(left, top, right-left, north->getHeight()); top += north->getHeight()+vgap_; }
        if (south && south->isVisible()) { bottom -= south->getHeight(); south->setBounds(left, bottom, right-left, south->getHeight()); bottom -= vgap_; }
        if (east && east->isVisible()) { right -= east->getWidth(); east->setBounds(right, top, east->getWidth(), bottom-top); right -= hgap_; }
        if (west && west->isVisible()) { west->setBounds(left, top, west->getWidth(), bottom-top); left += west->getWidth()+hgap_; }
        if (center && center->isVisible()) center->setBounds(left, top, right-left, bottom-top);
    }

    ::jxx::Ptr<::jxx::lang::String> BorderLayout::toString() const
    {
        std::ostringstream out;
        out << "jxx.awt.BorderLayout[hgap=" << hgap_
            << ",vgap=" << vgap_ << "]";
        return ::jxx::NEW<::jxx::lang::String>(out.str());
    }
}
