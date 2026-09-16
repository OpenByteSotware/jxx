#pragma once
#include "awt/event/jxx.awt.event.FocusListener.h"
#include "lang/jxx.lang.ClassInfo.h"
#include "lang/jxx.lang.Object.h"
namespace jxx::awt::event {
class FocusAdapter : public ::jxx::lang::ClassBase<FocusAdapter,::jxx::lang::Object,FocusListener> {
public:
 using JxxSuper=::jxx::lang::Object;
 using Super=::jxx::lang::ClassBase<FocusAdapter,JxxSuper,FocusListener>;
 FocusAdapter()=default; ~FocusAdapter() override=default;
 void focusGained(const ::jxx::Ptr<FocusEvent>& event) override;
 void focusLost(const ::jxx::Ptr<FocusEvent>& event) override;
}; }
