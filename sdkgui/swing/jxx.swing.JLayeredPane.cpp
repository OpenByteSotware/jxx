#include "swing/jxx.swing.JLayeredPane.h"

#include <algorithm>
#include <limits>

namespace jxx::swing
{
    JLayeredPane::JLayeredPane()
        : Super()
    {
        setLayout(nullptr);
        setOpaque(false);
    }

    void JLayeredPane::setLayer(
        const ::jxx::Ptr<::jxx::awt::Component>& component,
        ::jxx::lang::jint layer)
    {
        setLayer(component, layer, getPosition(component));
    }

    void JLayeredPane::setLayer(
        const ::jxx::Ptr<::jxx::awt::Component>& component,
        ::jxx::lang::jint layer,
        ::jxx::lang::jint position)
    {
        if (component == nullptr) return;
        layers_[component.get()] = LayerData{layer, position};
        invalidate();
    }

    ::jxx::lang::jint JLayeredPane::getLayer(
        const ::jxx::Ptr<::jxx::awt::Component>& component) const
    {
        if (component == nullptr) return DEFAULT_LAYER;
        const auto found = layers_.find(component.get());
        return found == layers_.end() ? DEFAULT_LAYER : found->second.layer;
    }

    ::jxx::lang::jint JLayeredPane::getPosition(
        const ::jxx::Ptr<::jxx::awt::Component>& component) const
    {
        if (component == nullptr) return -1;
        const auto found = layers_.find(component.get());
        return found == layers_.end() ? -1 : found->second.position;
    }

    void JLayeredPane::setPosition(
        const ::jxx::Ptr<::jxx::awt::Component>& component,
        ::jxx::lang::jint position)
    {
        if (component == nullptr) return;
        auto& data = layers_[component.get()];
        data.position = position;
        invalidate();
    }

    void JLayeredPane::moveToFront(
        const ::jxx::Ptr<::jxx::awt::Component>& component)
    {
        setPosition(component, 0);
    }

    void JLayeredPane::moveToBack(
        const ::jxx::Ptr<::jxx::awt::Component>& component)
    {
        setPosition(component, -1);
    }

    ::jxx::lang::jint JLayeredPane::highestLayer() const
    {
        if (layers_.empty()) return 0;
        auto value = std::numeric_limits<::jxx::lang::jint>::min();
        for (const auto& entry : layers_)
            value = std::max(value, entry.second.layer);
        return value;
    }

    ::jxx::lang::jint JLayeredPane::lowestLayer() const
    {
        if (layers_.empty()) return 0;
        auto value = std::numeric_limits<::jxx::lang::jint>::max();
        for (const auto& entry : layers_)
            value = std::min(value, entry.second.layer);
        return value;
    }

    ::jxx::lang::jint JLayeredPane::getComponentCountInLayer(
        ::jxx::lang::jint layer) const
    {
        ::jxx::lang::jint count = 0;
        for (const auto& entry : layers_)
            if (entry.second.layer == layer) ++count;
        return count;
    }
}
