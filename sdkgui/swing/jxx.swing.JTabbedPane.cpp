#include "swing/jxx.swing.JTabbedPane.h"
#include "lang/jxx.lang.IndexOutOfBoundsException.h"
#include "lang/jxx.lang.IllegalArgumentException.h"
#include "lang/jxx.lang.NullPointerException.h"
namespace jxx::swing
{
    JTabbedPane::JTabbedPane():JTabbedPane(TOP){} JTabbedPane::JTabbedPane(::jxx::lang::jint p):Super(){setLayout(nullptr);setTabPlacement(p);}
    void JTabbedPane::addTab(const ::jxx::Ptr<::jxx::lang::String>& t,const ::jxx::Ptr<::jxx::awt::Component>& c){insertTab(t,c,getTabCount());}
    void JTabbedPane::insertTab(const ::jxx::Ptr<::jxx::lang::String>& t,const ::jxx::Ptr<::jxx::awt::Component>& c,::jxx::lang::jint i){if(!c)throw ::jxx::lang::NullPointerException("component");if(i<0||i>getTabCount())throw ::jxx::lang::IndexOutOfBoundsException("index");tabs_.insert(tabs_.begin()+i,Tab{t,c});add(c);if(selectedIndex_<0)setSelectedIndex(0);else if(i<=selectedIndex_)++selectedIndex_;}
    void JTabbedPane::removeTabAt(::jxx::lang::jint i){if(i<0||i>=getTabCount())throw ::jxx::lang::IndexOutOfBoundsException("index");remove(tabs_[i].component);tabs_.erase(tabs_.begin()+i);if(tabs_.empty())selectedIndex_=-1;else setSelectedIndex(std::min(i,getTabCount()-1));}
    ::jxx::lang::jint JTabbedPane::getTabCount()const{return static_cast<::jxx::lang::jint>(tabs_.size());}
    ::jxx::Ptr<::jxx::lang::String> JTabbedPane::getTitleAt(::jxx::lang::jint i)const{if(i<0||i>=getTabCount())throw ::jxx::lang::IndexOutOfBoundsException("index");return tabs_[i].title;} void JTabbedPane::setTitleAt(::jxx::lang::jint i,const ::jxx::Ptr<::jxx::lang::String>& t){if(i<0||i>=getTabCount())throw ::jxx::lang::IndexOutOfBoundsException("index");tabs_[i].title=t;}
    ::jxx::Ptr<::jxx::awt::Component> JTabbedPane::getComponentAt(::jxx::lang::jint i)const{if(i<0||i>=getTabCount())throw ::jxx::lang::IndexOutOfBoundsException("index");return tabs_[i].component;}
    void JTabbedPane::setSelectedIndex(::jxx::lang::jint i){if(i<0||i>=getTabCount())throw ::jxx::lang::IndexOutOfBoundsException("index");selectedIndex_=i;for(::jxx::lang::jint n=0;n<getTabCount();++n)tabs_[n].component->setVisible(n==i);} ::jxx::lang::jint JTabbedPane::getSelectedIndex()const{return selectedIndex_;}
    ::jxx::Ptr<::jxx::awt::Component> JTabbedPane::getSelectedComponent()const{return selectedIndex_<0?nullptr:tabs_[selectedIndex_].component;}
    void JTabbedPane::setTabPlacement(::jxx::lang::jint p){if(p!=TOP&&p!=LEFT&&p!=BOTTOM&&p!=RIGHT)throw ::jxx::lang::IllegalArgumentException("placement");placement_=p;} ::jxx::lang::jint JTabbedPane::getTabPlacement()const{return placement_;}
}
