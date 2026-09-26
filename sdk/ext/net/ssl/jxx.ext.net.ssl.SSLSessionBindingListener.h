#pragma once
#include "lang/jxx.lang.ClassInfo.h"
namespace jxx::ext::net::ssl { class SSLSessionBindingEvent; class SSLSessionBindingListener:public ::jxx::lang::InterfaceBase<SSLSessionBindingListener>{public:~SSLSessionBindingListener()override=default;virtual void valueBound(const ::jxx::Ptr<SSLSessionBindingEvent>& event)=0;virtual void valueUnbound(const ::jxx::Ptr<SSLSessionBindingEvent>& event)=0;};}
