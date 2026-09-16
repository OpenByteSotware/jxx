#pragma once
#include "util/jxx.util.EventListener.h"
namespace jxx::awt::event { class WindowEvent; class WindowFocusListener : public ::jxx::lang::InterfaceBase<WindowFocusListener,::jxx::util::EventListener>{public:~WindowFocusListener() override=default;virtual void windowGainedFocus(const ::jxx::Ptr<WindowEvent>& event)=0;virtual void windowLostFocus(const ::jxx::Ptr<WindowEvent>& event)=0;}; }
