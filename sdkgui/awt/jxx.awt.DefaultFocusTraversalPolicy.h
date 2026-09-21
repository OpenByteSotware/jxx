#pragma once

#include "awt/jxx.awt.ContainerOrderFocusTraversalPolicy.h"

namespace jxx::awt
{
    class DefaultFocusTraversalPolicy :
        public ::jxx::lang::ClassBase<DefaultFocusTraversalPolicy,
            ContainerOrderFocusTraversalPolicy>
    {
    public:
        using JxxSuper = ContainerOrderFocusTraversalPolicy;
        using Super = ::jxx::lang::ClassBase<DefaultFocusTraversalPolicy,
            JxxSuper>;

        DefaultFocusTraversalPolicy();
        ~DefaultFocusTraversalPolicy() override = default;

    protected:
        ::jxx::lang::jbool accept(
            const ::jxx::Ptr<Component>& component) const override;
    };
}
