#include "swing/jxx.swing.DefaultListSelectionModel.h"
#include <algorithm>
#include "swing/event/jxx.swing.event.ListSelectionEvent.h"
#include "swing/event/jxx.swing.event.ListSelectionListener.h"
namespace jxx::swing
{
    DefaultListSelectionModel::DefaultListSelectionModel() : anchor_(-1), lead_(-1), mode_(MULTIPLE_INTERVAL_SELECTION), adjusting_(false) {}
    static void bounds_(::jxx::lang::jint a, ::jxx::lang::jint b, ::jxx::lang::jint& lo, ::jxx::lang::jint& hi) { lo=std::min(a,b); hi=std::max(a,b); }
    void DefaultListSelectionModel::setSelectionInterval(::jxx::lang::jint a, ::jxx::lang::jint b) { selected_.clear(); addSelectionInterval(a,b); }
    void DefaultListSelectionModel::addSelectionInterval(::jxx::lang::jint a, ::jxx::lang::jint b) { if(a<0||b<0)return; ::jxx::lang::jint lo,hi; bounds_(a,b,lo,hi); if(mode_==SINGLE_SELECTION){lo=hi=b;} else if(mode_==SINGLE_INTERVAL_SELECTION) selected_.clear(); for(auto i=lo;i<=hi;++i) selected_.insert(i); anchor_=a; lead_=b; fireValueChanged(lo,hi); }
    void DefaultListSelectionModel::removeSelectionInterval(::jxx::lang::jint a, ::jxx::lang::jint b) { ::jxx::lang::jint lo,hi; bounds_(a,b,lo,hi); for(auto i=lo;i<=hi;++i) selected_.erase(i); fireValueChanged(lo,hi); }
    ::jxx::lang::jint DefaultListSelectionModel::getMinSelectionIndex() const { return selected_.empty()?-1:*selected_.begin(); }
    ::jxx::lang::jint DefaultListSelectionModel::getMaxSelectionIndex() const { return selected_.empty()?-1:*selected_.rbegin(); }
    ::jxx::lang::jbool DefaultListSelectionModel::isSelectedIndex(::jxx::lang::jint i) const { return selected_.count(i)!=0; }
    ::jxx::lang::jint DefaultListSelectionModel::getAnchorSelectionIndex() const { return anchor_; }
    void DefaultListSelectionModel::setAnchorSelectionIndex(::jxx::lang::jint i) { anchor_=i; }
    ::jxx::lang::jint DefaultListSelectionModel::getLeadSelectionIndex() const { return lead_; }
    void DefaultListSelectionModel::setLeadSelectionIndex(::jxx::lang::jint i) { lead_=i; }
    void DefaultListSelectionModel::clearSelection() { if(selected_.empty())return; auto lo=getMinSelectionIndex(),hi=getMaxSelectionIndex(); selected_.clear(); fireValueChanged(lo,hi); }
    ::jxx::lang::jbool DefaultListSelectionModel::isSelectionEmpty() const { return selected_.empty(); }
    void DefaultListSelectionModel::insertIndexInterval(::jxx::lang::jint index, ::jxx::lang::jint length, ::jxx::lang::jbool before) { if(length<=0)return; auto start=index+(before?0:1); std::set<::jxx::lang::jint> next; for(auto i:selected_) next.insert(i>=start?i+length:i); selected_.swap(next); fireValueChanged(start,getMaxSelectionIndex()); }
    void DefaultListSelectionModel::removeIndexInterval(::jxx::lang::jint a, ::jxx::lang::jint b) { ::jxx::lang::jint lo,hi; bounds_(a,b,lo,hi); auto n=hi-lo+1; std::set<::jxx::lang::jint> next; for(auto i:selected_) if(i<lo) next.insert(i); else if(i>hi) next.insert(i-n); selected_.swap(next); fireValueChanged(lo,std::max(hi,getMaxSelectionIndex())); }
    void DefaultListSelectionModel::setValueIsAdjusting(::jxx::lang::jbool v) { if(adjusting_!=v){adjusting_=v; fireValueChanged(getMinSelectionIndex(),getMaxSelectionIndex());} }
    ::jxx::lang::jbool DefaultListSelectionModel::getValueIsAdjusting() const { return adjusting_; }
    void DefaultListSelectionModel::setSelectionMode(::jxx::lang::jint m) { if(m>=0&&m<=2) mode_=m; }
    ::jxx::lang::jint DefaultListSelectionModel::getSelectionMode() const { return mode_; }
    void DefaultListSelectionModel::addListSelectionListener(const ::jxx::Ptr<::jxx::swing::event::ListSelectionListener>& l) { if(l&&std::find(listeners_.begin(),listeners_.end(),l)==listeners_.end())listeners_.push_back(l); }
    void DefaultListSelectionModel::removeListSelectionListener(const ::jxx::Ptr<::jxx::swing::event::ListSelectionListener>& l) { listeners_.erase(std::remove(listeners_.begin(),listeners_.end(),l),listeners_.end()); }
    void DefaultListSelectionModel::fireValueChanged(::jxx::lang::jint first, ::jxx::lang::jint last) { auto e=::jxx::NEW<::jxx::swing::event::ListSelectionEvent>(::jxx::CAST<::jxx::lang::Object>(thisPtr()),first,last,adjusting_); auto copy=listeners_; for(const auto& l:copy)if(l)l->valueChanged(e); }
}
