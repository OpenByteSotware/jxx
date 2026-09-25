#include "ext/net/internal.DefaultSocketFactory.h"
#include "net/jxx.net.Socket.h"
namespace jxx::ext::net::internal {
::jxx::Ptr<::jxx::net::Socket> DefaultSocketFactory::createSocket(){return ::jxx::NEW<::jxx::net::Socket>();}
::jxx::Ptr<::jxx::net::Socket> DefaultSocketFactory::createSocket(const ::jxx::Ptr<::jxx::lang::String>&h,::jxx::lang::jint p){return ::jxx::NEW<::jxx::net::Socket>(h,p);}
::jxx::Ptr<::jxx::net::Socket> DefaultSocketFactory::createSocket(const ::jxx::Ptr<::jxx::lang::String>&h,::jxx::lang::jint p,const ::jxx::Ptr<::jxx::net::InetAddress>&l,::jxx::lang::jint lp){return ::jxx::NEW<::jxx::net::Socket>(h,p,l,lp);}
::jxx::Ptr<::jxx::net::Socket> DefaultSocketFactory::createSocket(const ::jxx::Ptr<::jxx::net::InetAddress>&h,::jxx::lang::jint p){return ::jxx::NEW<::jxx::net::Socket>(h,p);}
::jxx::Ptr<::jxx::net::Socket> DefaultSocketFactory::createSocket(const ::jxx::Ptr<::jxx::net::InetAddress>&h,::jxx::lang::jint p,const ::jxx::Ptr<::jxx::net::InetAddress>&l,::jxx::lang::jint lp){return ::jxx::NEW<::jxx::net::Socket>(h,p,l,lp);}}
