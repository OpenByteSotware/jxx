#pragma once
#include "lang/jxx.lang.ClassInfo.h"
#include "lang/jxx.lang.Object.h"
namespace jxx::util{template<class E>class List;}namespace jxx::lang{class String;}namespace jxx::com::sun::net::httpserver{class HttpExchange;class HttpHandler;class Filter:public ::jxx::lang::ClassBase<Filter,::jxx::lang::Object>{public:using JxxSuper=::jxx::lang::Object;using Super=::jxx::lang::ClassBase<Filter,JxxSuper>;class Chain;~Filter()override=default;virtual void doFilter(const ::jxx::Ptr<HttpExchange>&exchange,const ::jxx::Ptr<Chain>&chain)=0;virtual ::jxx::Ptr<::jxx::lang::String>description()=0;protected:Filter()=default;};class Filter::Chain final:public ::jxx::lang::ClassBase<Chain,::jxx::lang::Object>{public:using JxxSuper=::jxx::lang::Object;using Super=::jxx::lang::ClassBase<Chain,JxxSuper>;Chain(const ::jxx::Ptr<::jxx::util::List<Filter>>&filters,const ::jxx::Ptr<HttpHandler>&handler);void doFilter(const ::jxx::Ptr<HttpExchange>&exchange);private:
    ::jxx::Ptr<::jxx::util::List<Filter>> filters_;
    ::jxx::Ptr<HttpHandler>handler_;::jxx::lang::jint index_=0;};}
