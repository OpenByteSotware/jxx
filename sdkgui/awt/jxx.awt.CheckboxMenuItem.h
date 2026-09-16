#pragma once
#include <vector>
#include "awt/jxx.awt.ItemSelectable.h"
#include "awt/jxx.awt.MenuItem.h"
#include "lang/jxx.lang.buildin_array.h"
namespace jxx::awt::event { class ItemEvent; class ItemListener; }
namespace jxx::gui::internal { class WxFramePeer; }
namespace jxx::awt {
class CheckboxMenuItem : public ::jxx::lang::ClassBase<CheckboxMenuItem,MenuItem,ItemSelectable> {
public:
 using JxxSuper=MenuItem; using Super=::jxx::lang::ClassBase<CheckboxMenuItem,JxxSuper,ItemSelectable>;
 CheckboxMenuItem(); explicit CheckboxMenuItem(const ::jxx::Ptr<::jxx::lang::String>& label); CheckboxMenuItem(const ::jxx::Ptr<::jxx::lang::String>& label,::jxx::lang::jbool state);
 ::jxx::lang::jbool getState() const; void setState(::jxx::lang::jbool state);
 ::jxx::lang::ObjectArray getSelectedObjects() const override;
 void addItemListener(const ::jxx::Ptr<::jxx::awt::event::ItemListener>& listener) override;
 void removeItemListener(const ::jxx::Ptr<::jxx::awt::event::ItemListener>& listener) override;
protected: virtual void processItemEvent(const ::jxx::Ptr<::jxx::awt::event::ItemEvent>& event);
private: friend class ::jxx::gui::internal::WxFramePeer; void setStateFromNative(::jxx::lang::jbool state); ::jxx::lang::jbool state_; std::vector<::jxx::Ptr<::jxx::awt::event::ItemListener>> listeners_;
}; }
