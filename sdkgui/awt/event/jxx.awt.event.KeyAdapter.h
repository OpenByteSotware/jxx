#pragma once
#include "awt/event/jxx.awt.event.KeyListener.h"
#include "lang/jxx.lang.ClassInfo.h"
#include "lang/jxx.lang.Object.h"
namespace jxx::awt::event { class KeyAdapter : public ::jxx::lang::ClassBase<KeyAdapter,::jxx::lang::Object,KeyListener> { public: using JxxSuper=::jxx::lang::Object; using Super=::jxx::lang::ClassBase<KeyAdapter,JxxSuper,KeyListener>; KeyAdapter()=default; ~KeyAdapter() override=default; void keyTyped(const ::jxx::Ptr<KeyEvent>& event) override; void keyPressed(const ::jxx::Ptr<KeyEvent>& event) override; void keyReleased(const ::jxx::Ptr<KeyEvent>& event) override; }; }
