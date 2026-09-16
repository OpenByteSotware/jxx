#pragma once
#include "util/jxx.util.EventListener.h"
namespace jxx::awt::event { class WindowEvent; class WindowStateListener : public ::jxx::lang::InterfaceBase<WindowStateListener,::jxx::util::EventListener>{public:~WindowStateListener() override=default;virtual void windowStateChanged(const ::jxx::Ptr<WindowEvent>& event)=0;}; }
