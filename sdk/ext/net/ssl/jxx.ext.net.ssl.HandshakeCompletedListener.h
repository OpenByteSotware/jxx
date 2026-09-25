#pragma once
#include "lang/jxx.lang.ClassInfo.h"
namespace jxx::ext::net::ssl { class HandshakeCompletedEvent; class HandshakeCompletedListener:public ::jxx::lang::InterfaceBase<HandshakeCompletedListener>{public:~HandshakeCompletedListener()override=default;virtual void handshakeCompleted(const ::jxx::Ptr<HandshakeCompletedEvent>& event)=0;};}
