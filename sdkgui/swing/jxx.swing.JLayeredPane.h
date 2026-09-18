#pragma once

#include <unordered_map>

#include "swing/jxx.swing.JComponent.h"

namespace jxx::swing
{
    class JLayeredPane :
        public ::jxx::lang::ClassBase<JLayeredPane, JComponent>
    {
    public:
        using JxxSuper = JComponent;
        using Super = ::jxx::lang::ClassBase<JLayeredPane, JxxSuper>;

        static constexpr ::jxx::lang::jint FRAME_CONTENT_LAYER = -30000;
        static constexpr ::jxx::lang::jint DEFAULT_LAYER = 0;
        static constexpr ::jxx::lang::jint PALETTE_LAYER = 100;
        static constexpr ::jxx::lang::jint MODAL_LAYER = 200;
        static constexpr ::jxx::lang::jint POPUP_LAYER = 300;
        static constexpr ::jxx::lang::jint DRAG_LAYER = 400;

        JLayeredPane();
        ~JLayeredPane() override = default;

        void setLayer(const ::jxx::Ptr<::jxx::awt::Component>& component,
            ::jxx::lang::jint layer);
        void setLayer(const ::jxx::Ptr<::jxx::awt::Component>& component,
            ::jxx::lang::jint layer, ::jxx::lang::jint position);
        ::jxx::lang::jint getLayer(
            const ::jxx::Ptr<::jxx::awt::Component>& component) const;
        ::jxx::lang::jint getPosition(
            const ::jxx::Ptr<::jxx::awt::Component>& component) const;
        void setPosition(const ::jxx::Ptr<::jxx::awt::Component>& component,
            ::jxx::lang::jint position);
        void moveToFront(const ::jxx::Ptr<::jxx::awt::Component>& component);
        void moveToBack(const ::jxx::Ptr<::jxx::awt::Component>& component);
        ::jxx::lang::jint highestLayer() const;
        ::jxx::lang::jint lowestLayer() const;
        ::jxx::lang::jint getComponentCountInLayer(
            ::jxx::lang::jint layer) const;

    private:
        struct LayerData
        {
            ::jxx::lang::jint layer = DEFAULT_LAYER;
            ::jxx::lang::jint position = -1;
        };
        std::unordered_map<const ::jxx::awt::Component*, LayerData> layers_;
    };
}
