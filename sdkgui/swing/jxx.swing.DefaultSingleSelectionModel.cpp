#include "swing/jxx.swing.DefaultSingleSelectionModel.h"
#include <algorithm>
#include "swing/event/jxx.swing.event.ChangeEvent.h"
#include "swing/event/jxx.swing.event.ChangeListener.h"
namespace jxx::swing
{
    DefaultSingleSelectionModel::DefaultSingleSelectionModel() : index_(-1) {}
    ::jxx::lang::jint DefaultSingleSelectionModel::getSelectedIndex() const { return index_; }
    void DefaultSingleSelectionModel::setSelectedIndex(::jxx::lang::jint index)
    { if (index_ != index) { index_ = index; fireStateChanged(); } }
    void DefaultSingleSelectionModel::clearSelection() { setSelectedIndex(-1); }
    ::jxx::lang::jbool DefaultSingleSelectionModel::isSelected() const { return index_ != -1; }
    void DefaultSingleSelectionModel::addChangeListener(const ::jxx::Ptr<::jxx::swing::event::ChangeListener>& listener)
    { if (listener && std::find(listeners_.begin(), listeners_.end(), listener) == listeners_.end()) listeners_.push_back(listener); }
    void DefaultSingleSelectionModel::removeChangeListener(const ::jxx::Ptr<::jxx::swing::event::ChangeListener>& listener)
    { listeners_.erase(std::remove(listeners_.begin(), listeners_.end(), listener), listeners_.end()); }
    void DefaultSingleSelectionModel::fireStateChanged()
    { auto e=::jxx::NEW<::jxx::swing::event::ChangeEvent>(::jxx::CAST<::jxx::lang::Object>(thisPtr())); auto copy=listeners_; for(const auto& l:copy) if(l) l->stateChanged(e); }
}
