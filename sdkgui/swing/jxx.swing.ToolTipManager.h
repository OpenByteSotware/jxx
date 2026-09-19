#pragma once
#include <vector>
#include "swing/jxx.swing.JComponent.h"
namespace jxx::swing
{
    class ToolTipManager : public ::jxx::lang::ClassBase<ToolTipManager, ::jxx::lang::Object>
    {
    public:
        using JxxSuper = ::jxx::lang::Object;
        using Super = ::jxx::lang::ClassBase<ToolTipManager, JxxSuper>;
        static ::jxx::Ptr<ToolTipManager> sharedInstance();
        void registerComponent(const ::jxx::Ptr<JComponent>& component);
        void unregisterComponent(const ::jxx::Ptr<JComponent>& component);
        void setEnabled(::jxx::lang::jbool enabled) noexcept;
        ::jxx::lang::jbool isEnabled() const noexcept;
        void setLightWeightPopupEnabled(::jxx::lang::jbool enabled) noexcept;
        ::jxx::lang::jbool isLightWeightPopupEnabled() const noexcept;
        void setInitialDelay(::jxx::lang::jint delay);
        ::jxx::lang::jint getInitialDelay() const noexcept;
        void setDismissDelay(::jxx::lang::jint delay);
        ::jxx::lang::jint getDismissDelay() const noexcept;
        void setReshowDelay(::jxx::lang::jint delay);
        ::jxx::lang::jint getReshowDelay() const noexcept;
    private:
        std::vector<std::weak_ptr<JComponent>> components_;
        ::jxx::lang::jbool enabled_ = true, lightWeightPopupEnabled_ = true;
        ::jxx::lang::jint initialDelay_ = 750, dismissDelay_ = 4000, reshowDelay_ = 500;
    };
}
