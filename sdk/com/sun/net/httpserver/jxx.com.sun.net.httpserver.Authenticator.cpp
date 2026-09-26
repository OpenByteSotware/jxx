#include "com/sun/net/httpserver/jxx.com.sun.net.httpserver.Authenticator.h"
#include "com/sun/net/httpserver/jxx.com.sun.net.httpserver.HttpPrincipal.h"
#include "lang/jxx.lang.Exceptions.h"
namespace jxx::com::sun::net::httpserver { Authenticator::Success::Success(const ::jxx::Ptr<HttpPrincipal>&p):Super(),principal_(p){if(!p)throw ::jxx::lang::NullPointerException();}::jxx::Ptr<HttpPrincipal>Authenticator::Success::getPrincipal()const{return principal_;}Authenticator::Failure::Failure(::jxx::lang::jint c):Super(),code_(c){}::jxx::lang::jint Authenticator::Failure::getResponseCode()const noexcept{return code_;}Authenticator::Retry::Retry(::jxx::lang::jint c):Super(),code_(c){}::jxx::lang::jint Authenticator::Retry::getResponseCode()const noexcept{return code_;} }
