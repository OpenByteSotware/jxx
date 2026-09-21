#pragma once

#include "swing/jxx.swing.InputMap.h"

namespace jxx::swing
{
    class JComponent;

    class ComponentInputMap : public ::jxx::lang::ClassBase<
        ComponentInputMap, InputMap>
    {
    public:
        using JxxSuper = InputMap;
        using Super = ::jxx::lang::ClassBase<ComponentInputMap, JxxSuper>;

        explicit ComponentInputMap(
            const ::jxx::Ptr<JComponent>& component);
        ~ComponentInputMap() override = default;

        void setParent(const ::jxx::Ptr<InputMap>& map) override;
        ::jxx::Ptr<JComponent> getComponent() const;
        void put(
            const ::jxx::Ptr<KeyStroke>& keyStroke,
            const ::jxx::Ptr<::jxx::lang::Object>& actionMapKey) override;
        void remove(const ::jxx::Ptr<KeyStroke>& keyStroke) override;
        void clear() override;

    private:
        void notifyComponent();
        ::jxx::Ptr<JComponent> component_;
    };
}
