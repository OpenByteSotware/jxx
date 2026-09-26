#include <gtest/gtest.h>
#include "ext/net/ssl/jxx.ext.net.ssl.SSLSessionBindingEvent.h"
#include "ext/net/ssl/jxx.ext.net.ssl.SSLSessionBindingListener.h"
#include "ext/net/ssl/internal/jxx.ext.net.ssl.internal.OpenSslSession.h"
namespace { class Listener final:public ::jxx::lang::ClassBase<Listener,::jxx::lang::Object,::jxx::ext::net::ssl::SSLSessionBindingListener>{public:void valueBound(const ::jxx::Ptr<::jxx::ext::net::ssl::SSLSessionBindingEvent>&)override{++bound;}void valueUnbound(const ::jxx::Ptr<::jxx::ext::net::ssl::SSLSessionBindingEvent>&)override{++unbound;}int bound=0;int unbound=0;};}
TEST(SessionBindingListenerTest,InvalidationUnbindsValues){const auto s=::jxx::NEW<::jxx::ext::net::ssl::internal::OpenSslSession>(::jxx::NEW<::jxx::lang::String>("TLS_TEST"),::jxx::NEW<::jxx::lang::String>("TLSv1.2"),nullptr,-1);const auto l=::jxx::NEW<Listener>();s->putValue(::jxx::NEW<::jxx::lang::String>("name"),l);EXPECT_EQ(l->bound,1);s->invalidate();EXPECT_EQ(l->unbound,1);}
