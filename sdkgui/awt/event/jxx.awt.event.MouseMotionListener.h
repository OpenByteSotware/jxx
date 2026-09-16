#pragma once
#include "util/jxx.util.EventListener.h"
namespace jxx::awt::event { class MouseEvent; class MouseMotionListener : public ::jxx::lang::InterfaceBase<MouseMotionListener,::jxx::util::EventListener>{public:~MouseMotionListener() override=default;virtual void mouseDragged(const ::jxx::Ptr<MouseEvent>&)=0;virtual void mouseMoved(const ::jxx::Ptr<MouseEvent>&)=0;};}
