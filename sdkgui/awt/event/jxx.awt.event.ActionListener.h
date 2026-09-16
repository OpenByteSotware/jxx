#pragma once
#include "util/jxx.util.EventListener.h"
namespace jxx::awt::event { class ActionEvent; class ActionListener : public ::jxx::lang::InterfaceBase<ActionListener,::jxx::util::EventListener> { public: ~ActionListener() override=default; virtual void actionPerformed(const ::jxx::Ptr<ActionEvent>& event)=0; }; }
