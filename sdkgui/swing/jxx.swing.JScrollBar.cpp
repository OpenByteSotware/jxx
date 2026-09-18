#include "swing/jxx.swing.JScrollBar.h"
#include <algorithm>
#include "awt/event/jxx.awt.event.AdjustmentEvent.h"
#include "awt/event/jxx.awt.event.AdjustmentListener.h"
#include "lang/jxx.lang.IllegalArgumentException.h"
namespace jxx::swing
{
    JScrollBar::JScrollBar():JScrollBar(::jxx::awt::Adjustable::VERTICAL){}
    JScrollBar::JScrollBar(::jxx::lang::jint o):JScrollBar(o,0,10,0,100){}
    JScrollBar::JScrollBar(::jxx::lang::jint o,::jxx::lang::jint v,::jxx::lang::jint e,::jxx::lang::jint n,::jxx::lang::jint x):Super(),orientation_(o),value_(v),extent_(e),minimum_(n),maximum_(x){if(o!=0&&o!=1)throw ::jxx::lang::IllegalArgumentException("orientation");setValue(v);}
    ::jxx::lang::jint JScrollBar::getOrientation()const{return orientation_;}
    void JScrollBar::setMinimum(::jxx::lang::jint v){minimum_=v;setValue(value_);} ::jxx::lang::jint JScrollBar::getMinimum()const{return minimum_;}
    void JScrollBar::setMaximum(::jxx::lang::jint v){maximum_=std::max(v,minimum_+1);setValue(value_);} ::jxx::lang::jint JScrollBar::getMaximum()const{return maximum_;}
    void JScrollBar::setUnitIncrement(::jxx::lang::jint v){unitIncrement_=std::max(1,v);} ::jxx::lang::jint JScrollBar::getUnitIncrement()const{return unitIncrement_;}
    void JScrollBar::setBlockIncrement(::jxx::lang::jint v){blockIncrement_=std::max(1,v);} ::jxx::lang::jint JScrollBar::getBlockIncrement()const{return blockIncrement_;}
    void JScrollBar::setVisibleAmount(::jxx::lang::jint v){extent_=std::max(1,std::min(v,maximum_-minimum_));setValue(value_);} ::jxx::lang::jint JScrollBar::getVisibleAmount()const{return extent_;}
    void JScrollBar::setValue(::jxx::lang::jint v){value_=std::max(minimum_,std::min(v,maximum_-extent_));} ::jxx::lang::jint JScrollBar::getValue()const{return value_;}
    void JScrollBar::addAdjustmentListener(const ::jxx::Ptr<::jxx::awt::event::AdjustmentListener>& l){if(l&&std::find(listeners_.begin(),listeners_.end(),l)==listeners_.end())listeners_.push_back(l);}
    void JScrollBar::removeAdjustmentListener(const ::jxx::Ptr<::jxx::awt::event::AdjustmentListener>& l){listeners_.erase(std::remove(listeners_.begin(),listeners_.end(),l),listeners_.end());}
}
