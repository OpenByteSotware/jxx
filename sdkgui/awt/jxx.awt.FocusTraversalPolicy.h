#pragma once

#include "lang/jxx.lang.ClassInfo.h"
#include "lang/jxx.lang.Object.h"

namespace jxx::awt
{
    class Component;
    class Container;
    class Window;

    class FocusTraversalPolicy : public ::jxx::lang::ClassBase<
        FocusTraversalPolicy, ::jxx::lang::Object>
    {
    public:
        using JxxSuper = ::jxx::lang::Object;
        using Super = ::jxx::lang::ClassBase<FocusTraversalPolicy, JxxSuper>;

        FocusTraversalPolicy();
        ~FocusTraversalPolicy() override = default;

        virtual ::jxx::Ptr<Component> getComponentAfter(
            const ::jxx::Ptr<Container>& focusCycleRoot,
            const ::jxx::Ptr<Component>& component) const = 0;
        virtual ::jxx::Ptr<Component> getComponentBefore(
            const ::jxx::Ptr<Container>& focusCycleRoot,
            const ::jxx::Ptr<Component>& component) const = 0;
        virtual ::jxx::Ptr<Component> getFirstComponent(
            const ::jxx::Ptr<Container>& focusCycleRoot) const = 0;
        virtual ::jxx::Ptr<Component> getLastComponent(
            const ::jxx::Ptr<Container>& focusCycleRoot) const = 0;
        virtual ::jxx::Ptr<Component> getDefaultComponent(
            const ::jxx::Ptr<Container>& focusCycleRoot) const = 0;
        virtual ::jxx::Ptr<Component> getInitialComponent(
            const ::jxx::Ptr<Window>& window) const;
    };
}
