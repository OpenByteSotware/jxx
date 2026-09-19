#pragma once

#include <unordered_map>
#include "lang/jxx.lang.ClassInfo.h"
#include "lang/jxx.lang.Object.h"

namespace jxx::swing {
class Action;
class ActionMap final : public ::jxx::lang::ClassBase<ActionMap, ::jxx::lang::Object> {
public:
    using JxxSuper = ::jxx::lang::Object;
    using Super = ::jxx::lang::ClassBase<ActionMap, JxxSuper>;
    void setParent(const ::jxx::Ptr<ActionMap>& map);
    ::jxx::Ptr<ActionMap> getParent() const;
    void put(const ::jxx::Ptr<::jxx::lang::Object>& key, const ::jxx::Ptr<Action>& action);
    ::jxx::Ptr<Action> get(const ::jxx::Ptr<::jxx::lang::Object>& key) const;
    void remove(const ::jxx::Ptr<::jxx::lang::Object>& key);
    void clear();
    ::jxx::lang::jint size() const;
private:
    std::weak_ptr<ActionMap> parent_;
    std::unordered_map<const ::jxx::lang::Object*, ::jxx::Ptr<Action>> values_;
};
}
