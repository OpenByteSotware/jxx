#include "swing/jxx.swing.ActionMap.h"
#include "swing/jxx.swing.Action.h"
namespace jxx::swing {
void ActionMap::setParent(const ::jxx::Ptr<ActionMap>& map) { parent_ = map; }
::jxx::Ptr<ActionMap> ActionMap::getParent() const { return parent_.lock(); }
void ActionMap::put(const ::jxx::Ptr<::jxx::lang::Object>& key,
    const ::jxx::Ptr<Action>& action) {
    if (key == nullptr) return;
    if (action == nullptr) values_.erase(key.get()); else values_[key.get()] = action;
}
::jxx::Ptr<Action> ActionMap::get(const ::jxx::Ptr<::jxx::lang::Object>& key) const {
    if (key == nullptr) return nullptr;
    const auto found = values_.find(key.get());
    if (found != values_.end()) return found->second;
    const auto parent = parent_.lock();
    return parent == nullptr ? nullptr : parent->get(key);
}
void ActionMap::remove(const ::jxx::Ptr<::jxx::lang::Object>& key) {
    if (key != nullptr) values_.erase(key.get());
}
void ActionMap::clear() { values_.clear(); }
::jxx::lang::jint ActionMap::size() const {
    return static_cast<::jxx::lang::jint>(values_.size());
}
}
