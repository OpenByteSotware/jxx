#pragma once
#include "awt/event/jxx.awt.event.ComponentEvent.h"
namespace jxx::awt::event {
class FocusEvent : public ::jxx::lang::ClassBase<FocusEvent,ComponentEvent> {
public:
 using JxxSuper=ComponentEvent; using Super=::jxx::lang::ClassBase<FocusEvent,JxxSuper>;
 static constexpr ::jxx::lang::jint FOCUS_FIRST=1004;
 static constexpr ::jxx::lang::jint FOCUS_GAINED=1004;
 static constexpr ::jxx::lang::jint FOCUS_LOST=1005;
 static constexpr ::jxx::lang::jint FOCUS_LAST=1005;
 FocusEvent(const ::jxx::Ptr<::jxx::awt::Component>& source,::jxx::lang::jint id);
 FocusEvent(const ::jxx::Ptr<::jxx::awt::Component>& source,::jxx::lang::jint id,::jxx::lang::jbool temporary);
 FocusEvent(const ::jxx::Ptr<::jxx::awt::Component>& source,::jxx::lang::jint id,::jxx::lang::jbool temporary,const ::jxx::Ptr<::jxx::awt::Component>& opposite);
 ::jxx::lang::jbool isTemporary() const;
 ::jxx::Ptr<::jxx::awt::Component> getOppositeComponent() const;
protected: ::jxx::Ptr<::jxx::lang::String> paramString() const override;
private: ::jxx::lang::jbool temporary_; std::weak_ptr<::jxx::awt::Component> opposite_;
}; }
