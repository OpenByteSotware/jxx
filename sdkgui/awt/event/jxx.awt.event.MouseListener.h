#pragma once
#include "util/jxx.util.EventListener.h"
namespace jxx::awt::event { class MouseEvent; class MouseListener : public ::jxx::lang::InterfaceBase<MouseListener,::jxx::util::EventListener>{public:~MouseListener() override=default;virtual void mouseClicked(const ::jxx::Ptr<MouseEvent>&)=0;virtual void mousePressed(const ::jxx::Ptr<MouseEvent>&)=0;virtual void mouseReleased(const ::jxx::Ptr<MouseEvent>&)=0;virtual void mouseEntered(const ::jxx::Ptr<MouseEvent>&)=0;virtual void mouseExited(const ::jxx::Ptr<MouseEvent>&)=0;};}
