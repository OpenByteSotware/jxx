#pragma once
#include "util/jxx.util.EventListener.h"
namespace jxx::awt::event {
class FocusEvent;
class FocusListener : public ::jxx::lang::InterfaceBase<FocusListener,::jxx::util::EventListener> {
public:
 ~FocusListener() override=default;
 virtual void focusGained(const ::jxx::Ptr<FocusEvent>& event)=0;
 virtual void focusLost(const ::jxx::Ptr<FocusEvent>& event)=0;
}; }
