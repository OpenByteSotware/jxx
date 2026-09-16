#pragma once

#include "awt/jxx.awt.AWTEvent.h"
#include "awt/jxx.awt.Component.h"
#include "awt/jxx.awt.Container.h"

namespace jxx::awt::event
{
    class HierarchyEvent :
        public ::jxx::lang::ClassBase<HierarchyEvent, ::jxx::awt::AWTEvent>
    {
    public:
        using JxxSuper = ::jxx::awt::AWTEvent;
        using Super = ::jxx::lang::ClassBase<HierarchyEvent, JxxSuper>;

        static constexpr ::jxx::lang::jint HIERARCHY_FIRST = 1400;
        static constexpr ::jxx::lang::jint HIERARCHY_CHANGED = 1400;
        static constexpr ::jxx::lang::jint ANCESTOR_MOVED = 1401;
        static constexpr ::jxx::lang::jint ANCESTOR_RESIZED = 1402;
        static constexpr ::jxx::lang::jint HIERARCHY_LAST = 1402;

        static constexpr ::jxx::lang::jlong PARENT_CHANGED = 1;
        static constexpr ::jxx::lang::jlong DISPLAYABILITY_CHANGED = 2;
        static constexpr ::jxx::lang::jlong SHOWING_CHANGED = 4;

        HierarchyEvent(
            const ::jxx::Ptr<::jxx::awt::Component>& source,
            ::jxx::lang::jint id,
            const ::jxx::Ptr<::jxx::awt::Component>& changed,
            const ::jxx::Ptr<::jxx::awt::Container>& changedParent);

        HierarchyEvent(
            const ::jxx::Ptr<::jxx::awt::Component>& source,
            ::jxx::lang::jint id,
            const ::jxx::Ptr<::jxx::awt::Component>& changed,
            const ::jxx::Ptr<::jxx::awt::Container>& changedParent,
            ::jxx::lang::jlong changeFlags);

        ::jxx::Ptr<::jxx::awt::Component> getComponent() const;
        ::jxx::Ptr<::jxx::awt::Component> getChanged() const;
        ::jxx::Ptr<::jxx::awt::Container> getChangedParent() const;
        ::jxx::lang::jlong getChangeFlags() const;

    protected:
        ::jxx::Ptr<::jxx::lang::String> paramString() const override;

    private:
        ::jxx::Ptr<::jxx::awt::Component> changed_;
        ::jxx::Ptr<::jxx::awt::Container> changedParent_;
        ::jxx::lang::jlong changeFlags_;
    };
}
