#include "awt/jxx.awt.ContainerOrderFocusTraversalPolicy.h"

#include <algorithm>

#include "awt/jxx.awt.Component.h"
#include "awt/jxx.awt.Container.h"
#include "io/jxx.io.ObjectInputStream.h"
#include "io/jxx.io.ObjectOutputStream.h"
#include "lang/jxx.lang.IllegalArgumentException.h"

namespace jxx::awt
{
    ContainerOrderFocusTraversalPolicy::ContainerOrderFocusTraversalPolicy()
        : Super()
    {
    }

    void ContainerOrderFocusTraversalPolicy::appendCycle(
        const ::jxx::Ptr<Component>& component,
        std::vector<::jxx::Ptr<Component>>& cycle) const
    {
        if (component == nullptr)
            return;

        cycle.push_back(component);
        const auto container = ::jxx::CAST<Container>(component);
        if (container == nullptr)
            return;

        for (::jxx::lang::jint index = 0;
             index < container->getComponentCount(); ++index)
        {
            appendCycle(container->getComponent(index), cycle);
        }
    }

    std::vector<::jxx::Ptr<Component>>
    ContainerOrderFocusTraversalPolicy::createCycle(
        const ::jxx::Ptr<Container>& root) const
    {
        if (root == nullptr)
            throw ::jxx::lang::IllegalArgumentException("focusCycleRoot");

        std::vector<::jxx::Ptr<Component>> cycle;
        appendCycle(::jxx::CAST<Component>(root), cycle);
        return cycle;
    }

    ::jxx::Ptr<Component>
    ContainerOrderFocusTraversalPolicy::getComponentAfter(
        const ::jxx::Ptr<Container>& focusCycleRoot,
        const ::jxx::Ptr<Component>& component) const
    {
        if (component == nullptr)
            throw ::jxx::lang::IllegalArgumentException("component");

        const auto cycle = createCycle(focusCycleRoot);
        const auto current = std::find(cycle.begin(), cycle.end(), component);
        if (current == cycle.end())
            throw ::jxx::lang::IllegalArgumentException("component");

        for (auto next = current + 1; next != cycle.end(); ++next)
            if (accept(*next))
                return *next;
        for (auto next = cycle.begin(); next != current; ++next)
            if (accept(*next))
                return *next;
        return accept(component) ? component : nullptr;
    }

    ::jxx::Ptr<Component>
    ContainerOrderFocusTraversalPolicy::getComponentBefore(
        const ::jxx::Ptr<Container>& focusCycleRoot,
        const ::jxx::Ptr<Component>& component) const
    {
        if (component == nullptr)
            throw ::jxx::lang::IllegalArgumentException("component");

        const auto cycle = createCycle(focusCycleRoot);
        const auto current = std::find(cycle.begin(), cycle.end(), component);
        if (current == cycle.end())
            throw ::jxx::lang::IllegalArgumentException("component");

        auto previous = current;
        while (previous != cycle.begin())
        {
            --previous;
            if (accept(*previous))
                return *previous;
        }
        auto reverse = cycle.end();
        while (reverse != current + 1)
        {
            --reverse;
            if (accept(*reverse))
                return *reverse;
        }
        return accept(component) ? component : nullptr;
    }

    ::jxx::Ptr<Component>
    ContainerOrderFocusTraversalPolicy::getFirstComponent(
        const ::jxx::Ptr<Container>& focusCycleRoot) const
    {
        const auto cycle = createCycle(focusCycleRoot);
        for (const auto& component : cycle)
            if (accept(component))
                return component;
        return nullptr;
    }

    ::jxx::Ptr<Component>
    ContainerOrderFocusTraversalPolicy::getLastComponent(
        const ::jxx::Ptr<Container>& focusCycleRoot) const
    {
        const auto cycle = createCycle(focusCycleRoot);
        for (auto iterator = cycle.rbegin(); iterator != cycle.rend(); ++iterator)
            if (accept(*iterator))
                return *iterator;
        return nullptr;
    }

    ::jxx::Ptr<Component>
    ContainerOrderFocusTraversalPolicy::getDefaultComponent(
        const ::jxx::Ptr<Container>& focusCycleRoot) const
    {
        return getFirstComponent(focusCycleRoot);
    }

    void ContainerOrderFocusTraversalPolicy::setImplicitDownCycleTraversal(
        ::jxx::lang::jbool value)
    {
        implicitDownCycleTraversal_ = value;
    }

    ::jxx::lang::jbool
    ContainerOrderFocusTraversalPolicy::getImplicitDownCycleTraversal() const
    {
        return implicitDownCycleTraversal_;
    }

    ::jxx::lang::jbool ContainerOrderFocusTraversalPolicy::accept(
        const ::jxx::Ptr<Component>& component) const
    {
        return component != nullptr && component->isVisible() &&
            component->isEnabled();
    }

    void ContainerOrderFocusTraversalPolicy::writeObject(
        const ::jxx::Ptr<::jxx::io::ObjectOutputStream>& out)
    {
        out->writeBoolean(implicitDownCycleTraversal_);
    }

    void ContainerOrderFocusTraversalPolicy::readObject(
        const ::jxx::Ptr<::jxx::io::ObjectInputStream>& in)
    {
        implicitDownCycleTraversal_ = in->readBoolean();
    }

    void ContainerOrderFocusTraversalPolicy::readObjectNoData()
    {
        implicitDownCycleTraversal_ = true;
    }
}
