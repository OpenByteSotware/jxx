#pragma once
#include "util/jxx.util.EventListener.h"
namespace jxx::awt::event { class KeyEvent; class KeyListener : public ::jxx::lang::InterfaceBase<KeyListener,::jxx::util::EventListener> { public: ~KeyListener() override=default; virtual void keyTyped(const ::jxx::Ptr<KeyEvent>& event)=0; virtual void keyPressed(const ::jxx::Ptr<KeyEvent>& event)=0; virtual void keyReleased(const ::jxx::Ptr<KeyEvent>& event)=0; }; }
