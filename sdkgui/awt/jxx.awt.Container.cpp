#include "awt/jxx.awt.Container.h"

#include <algorithm>

#include "lang/jxx.lang.IllegalArgumentException.h"
#include "lang/jxx.lang.IndexOutOfBoundsException.h"
#include "lang/jxx.lang.NullPointerException.h"

namespace jxx::awt
{
    Container::Container() = default;
    Container::~Container() { removeAll(); }

    ::jxx::Ptr<Component> Container::add(const ::jxx::Ptr<Component>& component)
    {
        return add(component, getComponentCount());
    }

    ::jxx::Ptr<Component> Container::add(
        const ::jxx::Ptr<Component>& component,
        ::jxx::lang::jint index)
    {
        if (!component) throw ::jxx::lang::NullPointerException("component");
        if (index < 0 || index > getComponentCount())
            throw ::jxx::lang::IllegalArgumentException("index");
        if (auto old = component->getParent()) old->remove(component);
        components_.insert(components_.begin() + index, component);
        component->setParentInternal(::jxx::CAST<Container>(thisPtr()));
        if (layout_) layout_->addLayoutComponent(nullptr, component);
        invalidate();
        return component;
    }

    ::jxx::Ptr<Component> Container::add(
        const ::jxx::Ptr<::jxx::lang::String>& name,
        const ::jxx::Ptr<Component>& component)
    {
        auto result = add(component);
        if (layout_) layout_->addLayoutComponent(name, component);
        return result;
    }

    void Container::remove(::jxx::lang::jint index)
    {
        if (index < 0 || index >= getComponentCount())
            throw ::jxx::lang::IndexOutOfBoundsException("index");
        auto component = components_[static_cast<std::size_t>(index)];
        if (layout_) layout_->removeLayoutComponent(component);
        component->setNativeComponentInternal(nullptr);
        component->setParentInternal(nullptr);
        components_.erase(components_.begin() + index);
        invalidate();
    }

    void Container::remove(const ::jxx::Ptr<Component>& component)
    {
        auto it = std::find(components_.begin(), components_.end(), component);
        if (it != components_.end())
            remove(static_cast<::jxx::lang::jint>(
                std::distance(components_.begin(), it)));
    }

    void Container::removeAll()
    {
        while (!components_.empty()) remove(getComponentCount() - 1);
    }

    ::jxx::lang::jint Container::getComponentCount() const
    {
        return static_cast<::jxx::lang::jint>(components_.size());
    }

    ::jxx::Ptr<Component> Container::getComponent(::jxx::lang::jint index) const
    {
        if (index < 0 || index >= getComponentCount())
            throw ::jxx::lang::IndexOutOfBoundsException("index");
        return components_[static_cast<std::size_t>(index)];
    }

    void Container::setLayout(const ::jxx::Ptr<LayoutManager>& layout)
    {
        layout_ = layout;
        invalidate();
    }

    ::jxx::Ptr<LayoutManager> Container::getLayout() const { return layout_; }

    void Container::validate()
    {
        doLayout();
        for (const auto& component : components_)
        {
            if (auto container = ::jxx::CAST<Container>(component))
                container->validate();
            else if (component)
                component->validate();
        }
        Component::validate();
    }

    void Container::doLayout()
    {
        if (layout_) layout_->layoutContainer(::jxx::CAST<Container>(thisPtr()));
    }
}
