#include "awt/jxx.awt.Container.h"

#include "awt/jxx.awt.FocusTraversalPolicy.h"

#include <algorithm>

#include "awt/event/jxx.awt.event.ContainerEvent.h"
#include "awt/event/jxx.awt.event.ContainerListener.h"
#include "awt/event/jxx.awt.event.HierarchyEvent.h"
#include "lang/jxx.lang.IllegalArgumentException.h"
#include "lang/jxx.lang.IndexOutOfBoundsException.h"
#include "lang/jxx.lang.NullPointerException.h"

namespace jxx::awt
{
    Container::Container() = default;
    Container::~Container() { removeAll(); }

    ::jxx::Ptr<Component> Container::add(
        const ::jxx::Ptr<Component>& component)
    {
        return add(component, getComponentCount());
    }

    ::jxx::Ptr<Component> Container::add(
        const ::jxx::Ptr<Component>& component,
        ::jxx::lang::jint index)
    {
        if (!component)
            throw ::jxx::lang::NullPointerException("component");
        if (index < 0 || index > getComponentCount())
            throw ::jxx::lang::IllegalArgumentException("index");
        if (auto old = component->getParent()) old->remove(component);

        components_.insert(components_.begin() + index, component);
        component->setParentInternal(::jxx::CAST<Container>(thisPtr()));
        component->fireHierarchyEvent(::jxx::awt::event::HierarchyEvent::HIERARCHY_CHANGED, component, ::jxx::CAST<Container>(thisPtr()), ::jxx::awt::event::HierarchyEvent::PARENT_CHANGED);
        if (layout_) layout_->addLayoutComponent(nullptr, component);
        invalidate();
        fireContainerEvent(
            ::jxx::awt::event::ContainerEvent::COMPONENT_ADDED,
            component);
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
        component->fireHierarchyEvent(::jxx::awt::event::HierarchyEvent::HIERARCHY_CHANGED, component, ::jxx::CAST<Container>(thisPtr()), ::jxx::awt::event::HierarchyEvent::PARENT_CHANGED);
        components_.erase(components_.begin() + index);
        invalidate();
        fireContainerEvent(
            ::jxx::awt::event::ContainerEvent::COMPONENT_REMOVED,
            component);
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

    ::jxx::Ptr<Component> Container::getComponent(
        ::jxx::lang::jint index) const
    {
        if (index < 0 || index >= getComponentCount())
            throw ::jxx::lang::IndexOutOfBoundsException("index");
        return components_[static_cast<std::size_t>(index)];
    }

    void Container::addContainerListener(
        const ::jxx::Ptr<::jxx::awt::event::ContainerListener>& listener)
    {
        if (listener != nullptr &&
            std::find(containerListeners_.begin(), containerListeners_.end(), listener)
                == containerListeners_.end())
            containerListeners_.push_back(listener);
    }

    void Container::removeContainerListener(
        const ::jxx::Ptr<::jxx::awt::event::ContainerListener>& listener)
    {
        containerListeners_.erase(
            std::remove(containerListeners_.begin(), containerListeners_.end(), listener),
            containerListeners_.end());
    }

    void Container::fireContainerEvent(
        ::jxx::lang::jint id,
        const ::jxx::Ptr<Component>& child)
    {
        if (containerListeners_.empty()) return;
        processContainerEvent(::jxx::NEW<::jxx::awt::event::ContainerEvent>(
            ::jxx::CAST<Container>(thisPtr()), id, child));
    }

    void Container::processContainerEvent(
        const ::jxx::Ptr<::jxx::awt::event::ContainerEvent>& event)
    {
        if (event == nullptr) return;
        const auto listeners = containerListeners_;
        for (const auto& listener : listeners)
        {
            if (listener == nullptr) continue;
            if (event->getID() ==
                ::jxx::awt::event::ContainerEvent::COMPONENT_ADDED)
                listener->componentAdded(event);
            else if (event->getID() ==
                ::jxx::awt::event::ContainerEvent::COMPONENT_REMOVED)
                listener->componentRemoved(event);
        }
    }

    void Container::setLayout(const ::jxx::Ptr<LayoutManager>& layout)
    {
        layout_ = layout;
        invalidate();
    }

    ::jxx::Ptr<LayoutManager> Container::getLayout() const { return layout_; }
    void Container::applyComponentOrientation(
        const ::jxx::Ptr<ComponentOrientation>& orientation)
    {
        Component::applyComponentOrientation(orientation);
        const auto components = components_;
        for (const auto& component : components)
            if (component != nullptr)
                component->applyComponentOrientation(orientation);
    }

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

    void Container::fireHierarchyBoundsToDescendants(::jxx::lang::jint id, const ::jxx::Ptr<Component>& changed)
    {
        for (const auto& component : components_)
        {
            if (!component) continue;
            component->fireHierarchyEvent(id, changed, ::jxx::CAST<Container>(thisPtr()), 0);
            if (auto child = ::jxx::CAST<Container>(component)) child->fireHierarchyBoundsToDescendants(id, changed);
        }
    }
    void Container::doLayout()
    {
        if (layout_)
            layout_->layoutContainer(::jxx::CAST<Container>(thisPtr()));
    }
    void Container::setFocusTraversalPolicy(
        const ::jxx::Ptr<FocusTraversalPolicy>& policy)
    {
        focusTraversalPolicy_ = policy;
    }

    ::jxx::Ptr<FocusTraversalPolicy>
    Container::getFocusTraversalPolicy() const
    {
        return focusTraversalPolicy_;
    }

    void Container::setFocusCycleRoot(::jxx::lang::jbool value)
    {
        focusCycleRoot_ = value;
    }

    ::jxx::lang::jbool Container::isFocusCycleRoot() const
    {
        return focusCycleRoot_;
    }

    ::jxx::lang::jbool Container::isFocusCycleRoot(
        const ::jxx::Ptr<Container>& container) const
    {
        if (container == nullptr)
            return false;
        auto current = getParent();
        while (current != nullptr)
        {
            if (current == container)
                return container->isFocusCycleRoot();
            current = current->getParent();
        }
        return false;
    }

    void Container::setFocusTraversalPolicyProvider(
        ::jxx::lang::jbool value)
    {
        focusTraversalPolicyProvider_ = value;
    }

    ::jxx::lang::jbool
    Container::isFocusTraversalPolicyProvider() const
    {
        return focusTraversalPolicyProvider_;
    }

}
