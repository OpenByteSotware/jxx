#pragma once
#include "lang/jxx.lang.ClassInfo.h"
#include "lang/jxx.lang.Object.h"
#include "lang/jxx.lang.String.h"

namespace jxx::util {
class EventObject : public ::jxx::lang::ClassBase<EventObject, ::jxx::lang::Object> {
public:
 using JxxSuper=::jxx::lang::Object; using Super=::jxx::lang::ClassBase<EventObject,JxxSuper>;
 explicit EventObject(const ::jxx::Ptr<::jxx::lang::Object>& source);
 ~EventObject() override=default;
 ::jxx::Ptr<::jxx::lang::Object> getSource() const;
 ::jxx::Ptr<::jxx::lang::String> toString() const override;
protected: ::jxx::Ptr<::jxx::lang::Object> source;
}; }
