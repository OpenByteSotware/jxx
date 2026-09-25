#pragma once
#include "lang/jxx.lang.ClassInfo.h"
#include "lang/jxx.lang.buildin_array.h"
namespace jxx::ext::net::ssl {class SSLSession;class SSLSessionContext:public ::jxx::lang::InterfaceBase<SSLSessionContext>{public:virtual ~SSLSessionContext()override=default;virtual ::jxx::Ptr<SSLSession> getSession(const ::jxx::lang::ByteArray& id)=0;virtual ::jxx::lang::jint getSessionTimeout()const=0;virtual void setSessionTimeout(::jxx::lang::jint seconds)=0;virtual ::jxx::lang::jint getSessionCacheSize()const=0;virtual void setSessionCacheSize(::jxx::lang::jint size)=0;};}
