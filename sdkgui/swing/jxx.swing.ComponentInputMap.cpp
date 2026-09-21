#include "swing/jxx.swing.ComponentInputMap.h"

#include "lang/jxx.lang.IllegalArgumentException.h"
#include "swing/jxx.swing.JComponent.h"

namespace jxx::swing
{
    ComponentInputMap::ComponentInputMap(
        const ::jxx::Ptr<JComponent>& component)
        : Super(), component_(component)
    {
        if (component == nullptr)
            throw ::jxx::lang::IllegalArgumentException("component");
    }

    void ComponentInputMap::setParent(const ::jxx::Ptr<InputMap>& map)
    {
        if (map != nullptr)
        {
            const auto componentMap = ::jxx::CAST<ComponentInputMap>(map);
            if (componentMap == nullptr
                || componentMap->getComponent() != component_)
                throw ::jxx::lang::IllegalArgumentException("parent");
        }
        InputMap::setParent(map);
        notifyComponent();
    }

    ::jxx::Ptr<JComponent> ComponentInputMap::getComponent() const
    {
        return component_;
    }

    void ComponentInputMap::put(
        const ::jxx::Ptr<KeyStroke>& keyStroke,
        const ::jxx::Ptr<::jxx::lang::Object>& actionMapKey)
    {
        InputMap::put(keyStroke, actionMapKey);
        notifyComponent();
    }

    void ComponentInputMap::remove(
        const ::jxx::Ptr<KeyStroke>& keyStroke)
    {
        InputMap::remove(keyStroke);
        notifyComponent();
    }

    void ComponentInputMap::clear()
    {
        InputMap::clear();
        notifyComponent();
    }

    void ComponentInputMap::notifyComponent()
    {
        if (component_ != nullptr)
        {
            component_->revalidateInputMap(
                ::jxx::CAST<ComponentInputMap>(thisPtr()));
        }
    }
}
