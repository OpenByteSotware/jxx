#include "swing/jxx.swing.JPopupMenu.h"
#include "swing/jxx.swing.JMenuItem.h"
#include "lang/jxx.lang.IndexOutOfBoundsException.h"
#include "lang/jxx.lang.NullPointerException.h"
namespace jxx::swing
{
    namespace { ::jxx::Ptr<::jxx::lang::String> emptyLabel(){ return ::jxx::NEW<::jxx::lang::String>(""); } }
    JPopupMenu::JPopupMenu() : JPopupMenu(emptyLabel()) {}
    JPopupMenu::JPopupMenu(const ::jxx::Ptr<::jxx::lang::String>& label) : Super(), label_(label ? label : emptyLabel()) { JComponent::setVisible(false); }
    ::jxx::Ptr<JMenuItem> JPopupMenu::add(const ::jxx::Ptr<JMenuItem>& item){ if(!item) throw ::jxx::lang::NullPointerException("item"); items_.push_back(item); return item; }
    ::jxx::Ptr<JMenuItem> JPopupMenu::add(const ::jxx::Ptr<::jxx::lang::String>& text){ return add(::jxx::NEW<JMenuItem>(text)); }
    void JPopupMenu::addSeparator(){ items_.push_back(nullptr); }
    void JPopupMenu::insert(const ::jxx::Ptr<JMenuItem>& item, ::jxx::lang::jint index){ if(!item) throw ::jxx::lang::NullPointerException("item"); if(index<0||index>getComponentCount()) throw ::jxx::lang::IndexOutOfBoundsException("index"); items_.insert(items_.begin()+index,item); }
    void JPopupMenu::remove(::jxx::lang::jint index){ if(index<0||index>=getComponentCount()) throw ::jxx::lang::IndexOutOfBoundsException("index"); items_.erase(items_.begin()+index); }
    void JPopupMenu::removeAll(){ items_.clear(); }
    ::jxx::lang::jint JPopupMenu::getComponentCount() const { return static_cast<::jxx::lang::jint>(items_.size()); }
    ::jxx::Ptr<::jxx::awt::Component> JPopupMenu::getComponent(::jxx::lang::jint index) const { if(index<0||index>=getComponentCount()) throw ::jxx::lang::IndexOutOfBoundsException("index"); return items_[index]; }
    void JPopupMenu::setLabel(const ::jxx::Ptr<::jxx::lang::String>& value){ label_=value?value:emptyLabel(); }
    ::jxx::Ptr<::jxx::lang::String> JPopupMenu::getLabel() const { return label_; }
    void JPopupMenu::setInvoker(const ::jxx::Ptr<::jxx::awt::Component>& value){ invoker_=value; }
    ::jxx::Ptr<::jxx::awt::Component> JPopupMenu::getInvoker() const { return invoker_; }
    void JPopupMenu::show(const ::jxx::Ptr<::jxx::awt::Component>& invoker, ::jxx::lang::jint x, ::jxx::lang::jint y){ if(!invoker) throw ::jxx::lang::NullPointerException("invoker"); invoker_=invoker; popupX_=x; popupY_=y; setVisible(true); }
    void JPopupMenu::setVisible(::jxx::lang::jbool value){ JComponent::setVisible(value); }
    ::jxx::lang::jbool JPopupMenu::isVisible() const { return JComponent::isVisible(); }
}
