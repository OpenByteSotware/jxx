#pragma once
#include "awt/event/jxx.awt.event.ComponentEvent.h"
#include "awt/jxx.awt.Window.h"
namespace jxx::awt::event {
class WindowEvent : public ::jxx::lang::ClassBase<WindowEvent, ComponentEvent> {
public:
 using JxxSuper=ComponentEvent; using Super=::jxx::lang::ClassBase<WindowEvent,JxxSuper>;
 static constexpr ::jxx::lang::jint WINDOW_FIRST=200,WINDOW_OPENED=200,WINDOW_CLOSING=201,WINDOW_CLOSED=202,WINDOW_ICONIFIED=203,WINDOW_DEICONIFIED=204,WINDOW_ACTIVATED=205,WINDOW_DEACTIVATED=206,WINDOW_GAINED_FOCUS=207,WINDOW_LOST_FOCUS=208,WINDOW_STATE_CHANGED=209,WINDOW_LAST=209;
 WindowEvent(const ::jxx::Ptr<::jxx::awt::Window>& source,::jxx::lang::jint id);
 ::jxx::Ptr<::jxx::awt::Window> getWindow() const;
protected: ::jxx::Ptr<::jxx::lang::String> paramString() const override;
}; }
