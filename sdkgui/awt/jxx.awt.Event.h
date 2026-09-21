#pragma once
#include "lang/jxx.lang.ClassInfo.h"
#include "lang/jxx.lang.Object.h"
namespace jxx::awt {
class Event : public ::jxx::lang::ClassBase<Event,::jxx::lang::Object> {
public:
 using JxxSuper=::jxx::lang::Object; using Super=::jxx::lang::ClassBase<Event,JxxSuper>;
 static constexpr ::jxx::lang::jint SHIFT_MASK=1,CTRL_MASK=2,META_MASK=4,ALT_MASK=8;
 static constexpr ::jxx::lang::jint ACTION_EVENT=1001,GOT_FOCUS=1004,LOST_FOCUS=1005;
 static constexpr ::jxx::lang::jint KEY_PRESS=401,KEY_RELEASE=402,KEY_ACTION=403,KEY_ACTION_RELEASE=404;
 static constexpr ::jxx::lang::jint MOUSE_DOWN=501,MOUSE_UP=502,MOUSE_MOVE=503,MOUSE_ENTER=504,MOUSE_EXIT=505,MOUSE_DRAG=506;
 static constexpr ::jxx::lang::jint SCROLL_LINE_UP=601,SCROLL_LINE_DOWN=602,SCROLL_PAGE_UP=603,SCROLL_PAGE_DOWN=604,SCROLL_ABSOLUTE=605;
 static constexpr ::jxx::lang::jint LIST_SELECT=701,LIST_DESELECT=702,WINDOW_DESTROY=201,WINDOW_EXPOSE=202,WINDOW_ICONIFY=203,WINDOW_DEICONIFY=204,WINDOW_MOVED=205;
 Event(const ::jxx::Ptr<::jxx::lang::Object>& target,::jxx::lang::jlong when,::jxx::lang::jint id,::jxx::lang::jint x,::jxx::lang::jint y,::jxx::lang::jint key,::jxx::lang::jint modifiers,const ::jxx::Ptr<::jxx::lang::Object>& argument);
 Event(const ::jxx::Ptr<::jxx::lang::Object>& target,::jxx::lang::jint id,const ::jxx::Ptr<::jxx::lang::Object>& argument);
 ::jxx::lang::jbool shiftDown() const; ::jxx::lang::jbool controlDown() const; ::jxx::lang::jbool metaDown() const; void translate(::jxx::lang::jint deltaX,::jxx::lang::jint deltaY);
 ::jxx::Ptr<::jxx::lang::Object> target; ::jxx::lang::jlong when=0; ::jxx::lang::jint id=0,x=0,y=0,key=0,modifiers=0; ::jxx::Ptr<::jxx::lang::Object> arg; ::jxx::Ptr<Event> evt;
}; }
