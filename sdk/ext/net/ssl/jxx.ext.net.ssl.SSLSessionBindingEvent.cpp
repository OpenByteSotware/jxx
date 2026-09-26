#include "ext/net/ssl/jxx.ext.net.ssl.SSLSessionBindingEvent.h"
#include "ext/net/ssl/jxx.ext.net.ssl.SSLSession.h"
#include "io/jxx.io.ObjectInputStream.h"
#include "io/jxx.io.ObjectOutputStream.h"
#include "lang/jxx.lang.IllegalArgumentException.h"
#include "lang/jxx.lang.IllegalStateException.h"
#include "lang/jxx.lang.NullPointerException.h"
namespace jxx::ext::net::ssl { SSLSessionBindingEvent::SSLSessionBindingEvent(const ::jxx::Ptr<SSLSession>&s,const ::jxx::Ptr<::jxx::lang::String>&n):Super(::jxx::CAST<::jxx::lang::Object>(s)),name_(n){if(s==nullptr)throw ::jxx::lang::IllegalArgumentException();if(n==nullptr)throw ::jxx::lang::NullPointerException();}::jxx::Ptr<::jxx::lang::String>SSLSessionBindingEvent::getName()const{return name_;}::jxx::Ptr<SSLSession>SSLSessionBindingEvent::getSession()const{return ::jxx::CAST<SSLSession>(getSource());}void SSLSessionBindingEvent::writeObject(const ::jxx::Ptr<::jxx::io::ObjectOutputStream>&o){if(o==nullptr)throw ::jxx::lang::NullPointerException();o->writeObject(::jxx::CAST<::jxx::lang::Object>(getSession()));o->writeObject(::jxx::CAST<::jxx::lang::Object>(name_));}void SSLSessionBindingEvent::readObject(const ::jxx::Ptr<::jxx::io::ObjectInputStream>&i){if(i==nullptr)throw ::jxx::lang::NullPointerException();const auto s=::jxx::CAST<SSLSession>(i->readObject());name_=::jxx::CAST<::jxx::lang::String>(i->readObject());if(s==nullptr||name_==nullptr)throw ::jxx::lang::IllegalStateException();}void SSLSessionBindingEvent::readObjectNoData(){throw ::jxx::lang::IllegalStateException();}}
