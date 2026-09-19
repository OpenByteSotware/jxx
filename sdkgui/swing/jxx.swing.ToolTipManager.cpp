#include <algorithm>
#include <mutex>
#include "lang/jxx.lang.IllegalArgumentException.h"
#include "swing/jxx.swing.ToolTipManager.h"
namespace { std::mutex gToolTipMutex; ::jxx::Ptr<::jxx::swing::ToolTipManager> gToolTipManager; }
namespace jxx::swing
{
    ::jxx::Ptr<ToolTipManager> ToolTipManager::sharedInstance() { std::lock_guard<std::mutex> lock(gToolTipMutex); if (!gToolTipManager) gToolTipManager = ::jxx::NEW<ToolTipManager>(); return gToolTipManager; }
    void ToolTipManager::registerComponent(const ::jxx::Ptr<JComponent>& c) { if (!c) return; for (const auto& w : components_) if (w.lock() == c) return; components_.push_back(c); }
    void ToolTipManager::unregisterComponent(const ::jxx::Ptr<JComponent>& c) { components_.erase(std::remove_if(components_.begin(), components_.end(), [&](const std::weak_ptr<JComponent>& w){ auto p=w.lock(); return !p || p==c; }), components_.end()); }
    void ToolTipManager::setEnabled(::jxx::lang::jbool v) noexcept { enabled_=v; } ::jxx::lang::jbool ToolTipManager::isEnabled() const noexcept { return enabled_; }
    void ToolTipManager::setLightWeightPopupEnabled(::jxx::lang::jbool v) noexcept { lightWeightPopupEnabled_=v; } ::jxx::lang::jbool ToolTipManager::isLightWeightPopupEnabled() const noexcept { return lightWeightPopupEnabled_; }
    static void checkDelay(::jxx::lang::jint v) { if (v < 0) throw ::jxx::lang::IllegalArgumentException("delay"); }
    void ToolTipManager::setInitialDelay(::jxx::lang::jint v){checkDelay(v);initialDelay_=v;} ::jxx::lang::jint ToolTipManager::getInitialDelay() const noexcept{return initialDelay_;}
    void ToolTipManager::setDismissDelay(::jxx::lang::jint v){checkDelay(v);dismissDelay_=v;} ::jxx::lang::jint ToolTipManager::getDismissDelay() const noexcept{return dismissDelay_;}
    void ToolTipManager::setReshowDelay(::jxx::lang::jint v){checkDelay(v);reshowDelay_=v;} ::jxx::lang::jint ToolTipManager::getReshowDelay() const noexcept{return reshowDelay_;}
}
