#pragma once

#include <vector>

#include "awt/jxx.awt.FocusTraversalPolicy.h"
#include "io/jxx.io.SerializableI.h"

namespace jxx::io
{
    class ObjectInputStream;
    class ObjectOutputStream;
}

namespace jxx::awt
{
    class ContainerOrderFocusTraversalPolicy :
        public ::jxx::lang::ClassBase<ContainerOrderFocusTraversalPolicy,
            FocusTraversalPolicy, ::jxx::io::SerializableI>
    {
    public:
        using JxxSuper = FocusTraversalPolicy;
        using Super = ::jxx::lang::ClassBase<
            ContainerOrderFocusTraversalPolicy, JxxSuper,
            ::jxx::io::SerializableI>;

        ContainerOrderFocusTraversalPolicy();
        ~ContainerOrderFocusTraversalPolicy() override = default;

        ::jxx::Ptr<Component> getComponentAfter(
            const ::jxx::Ptr<Container>& focusCycleRoot,
            const ::jxx::Ptr<Component>& component) const override;
        ::jxx::Ptr<Component> getComponentBefore(
            const ::jxx::Ptr<Container>& focusCycleRoot,
            const ::jxx::Ptr<Component>& component) const override;
        ::jxx::Ptr<Component> getFirstComponent(
            const ::jxx::Ptr<Container>& focusCycleRoot) const override;
        ::jxx::Ptr<Component> getLastComponent(
            const ::jxx::Ptr<Container>& focusCycleRoot) const override;
        ::jxx::Ptr<Component> getDefaultComponent(
            const ::jxx::Ptr<Container>& focusCycleRoot) const override;

        void setImplicitDownCycleTraversal(::jxx::lang::jbool value);
        ::jxx::lang::jbool getImplicitDownCycleTraversal() const;

        void writeObject(
            const ::jxx::Ptr<::jxx::io::ObjectOutputStream>& out) override;
        void readObject(
            const ::jxx::Ptr<::jxx::io::ObjectInputStream>& in) override;
        void readObjectNoData() override;

    protected:
        virtual ::jxx::lang::jbool accept(
            const ::jxx::Ptr<Component>& component) const;

    private:
        void appendCycle(const ::jxx::Ptr<Component>& component,
            std::vector<::jxx::Ptr<Component>>& cycle) const;
        std::vector<::jxx::Ptr<Component>> createCycle(
            const ::jxx::Ptr<Container>& root) const;

        ::jxx::lang::jbool implicitDownCycleTraversal_ = true;
    };
}
