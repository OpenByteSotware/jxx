#pragma once
#include "lang/jxx.lang.ClassInfo.h"
#include "awt/jxx.awt.AWTEvent.h"
namespace jxx::awt::event {
class ActionEvent : public ::jxx::lang::ClassBase<ActionEvent, ::jxx::awt::AWTEvent> {
public:
 using JxxSuper=::jxx::awt::AWTEvent; using Super=::jxx::lang::ClassBase<ActionEvent,JxxSuper>;
 static constexpr ::jxx::lang::jint SHIFT_MASK=1,CTRL_MASK=2,META_MASK=4,ALT_MASK=8,ACTION_FIRST=1001,ACTION_PERFORMED=1001,ACTION_LAST=1001;
 ActionEvent(const ::jxx::Ptr<::jxx::lang::Object>& source,::jxx::lang::jint id,const ::jxx::Ptr<::jxx::lang::String>& command);
 ActionEvent(const ::jxx::Ptr<::jxx::lang::Object>& source,::jxx::lang::jint id,const ::jxx::Ptr<::jxx::lang::String>& command,::jxx::lang::jint modifiers);
 ActionEvent(const ::jxx::Ptr<::jxx::lang::Object>& source,::jxx::lang::jint id,const ::jxx::Ptr<::jxx::lang::String>& command,::jxx::lang::jlong when,::jxx::lang::jint modifiers);
 ::jxx::Ptr<::jxx::lang::String> getActionCommand() const; ::jxx::lang::jlong getWhen() const; ::jxx::lang::jint getModifiers() const;
protected: ::jxx::Ptr<::jxx::lang::String> paramString() const override;
private: ::jxx::Ptr<::jxx::lang::String> command_; ::jxx::lang::jlong when_; ::jxx::lang::jint modifiers_;
}; }
