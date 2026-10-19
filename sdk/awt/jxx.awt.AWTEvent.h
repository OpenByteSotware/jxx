#pragma once
#include "lang/jxx.lang.ClassInfo.h"
#include "util/jxx.util.EventObject.h"

namespace jxx::awt {
class AWTEvent : public ::jxx::lang::ClassBase<AWTEvent, ::jxx::util::EventObject> {
public:
 using JxxSuper=::jxx::util::EventObject; using Super=::jxx::lang::ClassBase<AWTEvent,JxxSuper>;
 static constexpr ::jxx::lang::jint RESERVED_ID_MAX=1999;
 AWTEvent(const ::jxx::Ptr<::jxx::lang::Object>& source,::jxx::lang::jint id);
 ~AWTEvent() override=default;
 ::jxx::lang::jint getID() const;
 ::jxx::lang::jbool isConsumed() const;
 ::jxx::Ptr<::jxx::lang::String> toString() const override;
protected:
 void consume(); virtual ::jxx::Ptr<::jxx::lang::String> paramString() const;
 ::jxx::lang::jint id; ::jxx::lang::jbool consumed;
}; }
