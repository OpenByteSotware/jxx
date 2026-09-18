#include "swing/jxx.swing.JMenu.h"
#include <algorithm>
#include "lang/jxx.lang.IllegalArgumentException.h"
#include "lang/jxx.lang.NullPointerException.h"
namespace jxx::swing
{
    JMenu::JMenu() : Super() {}
    JMenu::JMenu(const ::jxx::Ptr<::jxx::lang::String>& text) : Super(text) {}
    ::jxx::Ptr<JMenuItem> JMenu::add(const ::jxx::Ptr<JMenuItem>& item) { if (!item) throw ::jxx::lang::NullPointerException("item"); items_.push_back(item); return item; }
    ::jxx::Ptr<JMenuItem> JMenu::add(const ::jxx::Ptr<::jxx::lang::String>& text) { return add(::jxx::NEW<JMenuItem>(text)); }
    void JMenu::addSeparator() { items_.push_back(nullptr); }
    void JMenu::insert(const ::jxx::Ptr<JMenuItem>& item, ::jxx::lang::jint position) { if (!item) throw ::jxx::lang::NullPointerException("item"); if (position < 0 || position > static_cast<::jxx::lang::jint>(items_.size())) throw ::jxx::lang::IllegalArgumentException("position"); items_.insert(items_.begin() + position, item); }
    void JMenu::remove(::jxx::lang::jint position) { if (position < 0 || position >= static_cast<::jxx::lang::jint>(items_.size())) throw ::jxx::lang::IllegalArgumentException("position"); items_.erase(items_.begin() + position); }
    void JMenu::remove(const ::jxx::Ptr<JMenuItem>& item) { items_.erase(std::remove(items_.begin(), items_.end(), item), items_.end()); }
    void JMenu::removeAll() { items_.clear(); }
    ::jxx::lang::jint JMenu::getItemCount() const { return static_cast<::jxx::lang::jint>(items_.size()); }
    ::jxx::Ptr<JMenuItem> JMenu::getItem(::jxx::lang::jint position) const { if (position < 0 || position >= static_cast<::jxx::lang::jint>(items_.size())) throw ::jxx::lang::IllegalArgumentException("position"); return items_[position]; }
    ::jxx::lang::jbool JMenu::isTopLevelMenu() const { return getParent() != nullptr; }
    void JMenu::setPopupMenuVisible(::jxx::lang::jbool value) { popupVisible_ = value; }
    ::jxx::lang::jbool JMenu::isPopupMenuVisible() const { return popupVisible_; }
}
