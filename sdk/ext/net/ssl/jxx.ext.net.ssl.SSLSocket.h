#pragma once
#include "net/jxx.net.Socket.h"
#include "lang/jxx.lang.buildin_array.h"
namespace jxx::ext::net::ssl { class SSLSession; class HandshakeCompletedListener;
class SSLSocket:public ::jxx::lang::ClassBase<SSLSocket,::jxx::net::Socket>{public:
using StringArray=::jxx::lang::JxxArray<::jxx::Ptr<::jxx::lang::String>,1U>; using JxxSuper=::jxx::net::Socket; using Super=::jxx::lang::ClassBase<SSLSocket,JxxSuper>;
virtual ~SSLSocket()override=default; virtual ::jxx::Ptr<StringArray> getSupportedCipherSuites()const=0;virtual ::jxx::Ptr<StringArray> getEnabledCipherSuites()const=0;virtual void setEnabledCipherSuites(const ::jxx::Ptr<StringArray>&)=0;virtual ::jxx::Ptr<StringArray> getSupportedProtocols()const=0;virtual ::jxx::Ptr<StringArray> getEnabledProtocols()const=0;virtual void setEnabledProtocols(const ::jxx::Ptr<StringArray>&)=0;virtual ::jxx::Ptr<SSLSession> getSession()=0;virtual void addHandshakeCompletedListener(const ::jxx::Ptr<HandshakeCompletedListener>&)=0;virtual void removeHandshakeCompletedListener(const ::jxx::Ptr<HandshakeCompletedListener>&)=0;virtual void startHandshake()=0;virtual void setUseClientMode(::jxx::lang::jbool)=0;virtual ::jxx::lang::jbool getUseClientMode()const=0;virtual void setNeedClientAuth(::jxx::lang::jbool)=0;virtual ::jxx::lang::jbool getNeedClientAuth()const=0;virtual void setWantClientAuth(::jxx::lang::jbool)=0;virtual ::jxx::lang::jbool getWantClientAuth()const=0;virtual void setEnableSessionCreation(::jxx::lang::jbool)=0;virtual ::jxx::lang::jbool getEnableSessionCreation()const=0;
protected:SSLSocket()=default;};}
