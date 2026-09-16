#pragma once
#include "util/jxx.util.EventListener.h"
namespace jxx::awt::event { class TextEvent; class TextListener : public ::jxx::lang::InterfaceBase<TextListener,::jxx::util::EventListener>{public:~TextListener() override=default;virtual void textValueChanged(const ::jxx::Ptr<TextEvent>& event)=0;}; }
