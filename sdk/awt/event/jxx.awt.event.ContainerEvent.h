#pragma once

#include "awt/event/jxx.awt.event.ComponentEvent.h"

namespace jxx::awt
{
    class Component;
    class Container;
}

namespace jxx::awt::event
{
    class ContainerEvent :
        public ::jxx::lang::ClassBase<ContainerEvent, ComponentEvent>
    {
    public:
        using JxxSuper = ComponentEvent;
        using Super = ::jxx::lang::ClassBase<ContainerEvent, JxxSuper>;

        static constexpr ::jxx::lang::jint CONTAINER_FIRST = 300;
        static constexpr ::jxx::lang::jint COMPONENT_ADDED = 300;
        static constexpr ::jxx::lang::jint COMPONENT_REMOVED = 301;
        static constexpr ::jxx::lang::jint CONTAINER_LAST = 301;

        ContainerEvent(
            const ::jxx::Ptr<::jxx::awt::Container>& source,
            ::jxx::lang::jint id,
            const ::jxx::Ptr<::jxx::awt::Component>& child);

        ::jxx::Ptr<::jxx::awt::Container> getContainer() const;
        ::jxx::Ptr<::jxx::awt::Component> getChild() const;

    protected:
        ::jxx::Ptr<::jxx::lang::String> paramString() const override;

    private:
        ::jxx::Ptr<::jxx::awt::Component> child_;
    };
}
