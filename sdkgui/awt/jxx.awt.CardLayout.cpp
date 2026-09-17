#include "awt/jxx.awt.CardLayout.h"

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
    CardLayout::CardLayout() : CardLayout(0, 0) {}
    CardLayout::CardLayout(::jxx::lang::jint hgap,
        ::jxx::lang::jint vgap) : hgap_(hgap), vgap_(vgap) {}

    ::jxx::lang::jint CardLayout::getHgap() const { return hgap_; }
    void CardLayout::setHgap(::jxx::lang::jint value) { hgap_ = value; }
    ::jxx::lang::jint CardLayout::getVgap() const { return vgap_; }
    void CardLayout::setVgap(::jxx::lang::jint value) { vgap_ = value; }

    void CardLayout::addLayoutComponent(
        const ::jxx::Ptr<::jxx::lang::String>& name,
        const ::jxx::Ptr<Component>& component)
    {
        addLayoutComponent(component,
            ::jxx::CAST<::jxx::lang::Object>(name));
    }

    void CardLayout::addLayoutComponent(
        const ::jxx::Ptr<Component>& component,
        const ::jxx::Ptr<::jxx::lang::Object>& constraints)
    {
        const auto name = constraints == nullptr
            ? ::jxx::NEW<::jxx::lang::String>("")
            : ::jxx::CAST<::jxx::lang::String>(constraints);
        if (name == nullptr)
            throw ::jxx::lang::IllegalArgumentException("constraints");
        namedCards_[name->utf8()] = component;
    }

    void CardLayout::removeLayoutComponent(
        const ::jxx::Ptr<Component>& component)
    {
        for (auto iterator = namedCards_.begin(); iterator != namedCards_.end();)
            if (iterator->second == component) iterator = namedCards_.erase(iterator);
            else ++iterator;
    }

    ::jxx::Ptr<Dimension> CardLayout::preferredLayoutSize(
        const ::jxx::Ptr<Container>& parent)
    {
        ::jxx::lang::jint width = 0;
        ::jxx::lang::jint height = 0;
        if (parent != nullptr)
            for (::jxx::lang::jint index = 0;
                index < parent->getComponentCount(); ++index)
            {
                const auto component = parent->getComponent(index);
                if (component == nullptr) continue;
                width = std::max(width, component->getWidth());
                height = std::max(height, component->getHeight());
            }
        return ::jxx::NEW<Dimension>(width + 2 * hgap_, height + 2 * vgap_);
    }

    ::jxx::Ptr<Dimension> CardLayout::minimumLayoutSize(
        const ::jxx::Ptr<Container>& parent)
    {
        return preferredLayoutSize(parent);
    }

    ::jxx::Ptr<Dimension> CardLayout::maximumLayoutSize(
        const ::jxx::Ptr<Container>&)
    {
        const auto maximum = std::numeric_limits<::jxx::lang::jint>::max();
        return ::jxx::NEW<Dimension>(maximum, maximum);
    }

    ::jxx::lang::jfloat CardLayout::getLayoutAlignmentX(
        const ::jxx::Ptr<Container>&) { return 0.5f; }
    ::jxx::lang::jfloat CardLayout::getLayoutAlignmentY(
        const ::jxx::Ptr<Container>&) { return 0.5f; }
    void CardLayout::invalidateLayout(const ::jxx::Ptr<Container>&) {}

    ::jxx::lang::jint CardLayout::visibleIndex(
        const ::jxx::Ptr<Container>& parent) const
    {
        if (parent == nullptr) return -1;
        for (::jxx::lang::jint index = 0;
            index < parent->getComponentCount(); ++index)
            if (parent->getComponent(index)->isVisible()) return index;
        return -1;
    }

    void CardLayout::showIndex(const ::jxx::Ptr<Container>& parent,
        ::jxx::lang::jint index)
    {
        if (parent == nullptr || parent->getComponentCount() == 0) return;
        const auto count = parent->getComponentCount();
        index = (index % count + count) % count;
        for (::jxx::lang::jint current = 0; current < count; ++current)
            parent->getComponent(current)->setVisible(current == index);
        parent->validate();
    }

    void CardLayout::layoutContainer(const ::jxx::Ptr<Container>& parent)
    {
        if (parent == nullptr) return;
        for (::jxx::lang::jint index = 0;
            index < parent->getComponentCount(); ++index)
            parent->getComponent(index)->setBounds(hgap_, vgap_,
                std::max<::jxx::lang::jint>(0, parent->getWidth() - 2 * hgap_),
                std::max<::jxx::lang::jint>(0, parent->getHeight() - 2 * vgap_));
        if (visibleIndex(parent) < 0 && parent->getComponentCount() > 0)
            showIndex(parent, 0);
    }

    void CardLayout::first(const ::jxx::Ptr<Container>& parent)
    {
        showIndex(parent, 0);
    }
    void CardLayout::next(const ::jxx::Ptr<Container>& parent)
    {
        showIndex(parent, visibleIndex(parent) + 1);
    }
    void CardLayout::previous(const ::jxx::Ptr<Container>& parent)
    {
        const auto current = visibleIndex(parent);
        showIndex(parent, current < 0 ? 0 : current - 1);
    }
    void CardLayout::last(const ::jxx::Ptr<Container>& parent)
    {
        if (parent != nullptr) showIndex(parent, parent->getComponentCount() - 1);
    }
    void CardLayout::show(const ::jxx::Ptr<Container>& parent,
        const ::jxx::Ptr<::jxx::lang::String>& name)
    {
        if (parent == nullptr || name == nullptr) return;
        const auto iterator = namedCards_.find(name->utf8());
        if (iterator == namedCards_.end()) return;
        for (::jxx::lang::jint index = 0;
            index < parent->getComponentCount(); ++index)
            if (parent->getComponent(index) == iterator->second)
            {
                showIndex(parent, index);
                return;
            }
    }

    ::jxx::Ptr<::jxx::lang::String> CardLayout::toString() const
    {
        std::ostringstream out;
        out << "jxx.awt.CardLayout[hgap=" << hgap_
            << ",vgap=" << vgap_ << "]";
        return ::jxx::NEW<::jxx::lang::String>(out.str());
    }
}
