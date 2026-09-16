#pragma once
#include "awt/jxx.awt.AWTEvent.h"
#include "awt/jxx.awt.Component.h"
namespace jxx::awt::event {
class ComponentEvent : public ::jxx::lang::ClassBase<ComponentEvent, ::jxx::awt::AWTEvent> {
public:
 using JxxSuper=::jxx::awt::AWTEvent; using Super=::jxx::lang::ClassBase<ComponentEvent,JxxSuper>;
 static constexpr ::jxx::lang::jint COMPONENT_FIRST=100,COMPONENT_MOVED=100,COMPONENT_RESIZED=101,COMPONENT_SHOWN=102,COMPONENT_HIDDEN=103,COMPONENT_LAST=103;
 ComponentEvent(const ::jxx::Ptr<::jxx::awt::Component>& source,::jxx::lang::jint id);
 ::jxx::Ptr<::jxx::awt::Component> getComponent() const;
protected: ::jxx::Ptr<::jxx::lang::String> paramString() const override;
}; }
