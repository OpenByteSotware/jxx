#pragma once

#include <vector>

#include "awt/jxx.awt.Component.h"
#include "awt/jxx.awt.LayoutManager.h"

namespace jxx::awt::event
{
    class ContainerEvent;
    class ContainerListener;
}

namespace jxx::awt
{
    class Container :
        public ::jxx::lang::ClassBase<Container, Component>
    {
    public:
        using JxxSuper = Component;
        using Super = ::jxx::lang::ClassBase<Container, JxxSuper>;

        Container();
        ~Container() override;

        virtual ::jxx::Ptr<Component> add(
            const ::jxx::Ptr<Component>& component);
        virtual ::jxx::Ptr<Component> add(
            const ::jxx::Ptr<Component>& component,
            ::jxx::lang::jint index);
        virtual ::jxx::Ptr<Component> add(
            const ::jxx::Ptr<::jxx::lang::String>& name,
            const ::jxx::Ptr<Component>& component);

        virtual void remove(::jxx::lang::jint index);
        virtual void remove(const ::jxx::Ptr<Component>& component);
        virtual void removeAll();

        virtual ::jxx::lang::jint getComponentCount() const;
        virtual ::jxx::Ptr<Component> getComponent(
            ::jxx::lang::jint index) const;

        virtual void addContainerListener(
            const ::jxx::Ptr<::jxx::awt::event::ContainerListener>& listener);
        virtual void removeContainerListener(
            const ::jxx::Ptr<::jxx::awt::event::ContainerListener>& listener);

        virtual void setLayout(const ::jxx::Ptr<LayoutManager>& layout);
        virtual ::jxx::Ptr<LayoutManager> getLayout() const;
        void validate() override;
        virtual void doLayout();

    protected:
        virtual void processContainerEvent(
            const ::jxx::Ptr<::jxx::awt::event::ContainerEvent>& event);

    private:
        friend class Component;
        void fireHierarchyBoundsToDescendants(::jxx::lang::jint id, const ::jxx::Ptr<Component>& changed);
        void fireContainerEvent(
            ::jxx::lang::jint id,
            const ::jxx::Ptr<Component>& child);

        std::vector<::jxx::Ptr<Component>> components_;
        ::jxx::Ptr<LayoutManager> layout_;
        std::vector<::jxx::Ptr<::jxx::awt::event::ContainerListener>>
            containerListeners_;
    };
}
