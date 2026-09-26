#pragma once
#include "lang/jxx.lang.ClassInfo.h"
namespace jxx::com::sun::net::httpserver {
class HttpExchange;
class HttpHandler : public ::jxx::lang::InterfaceBase<HttpHandler> {
public:
    ~HttpHandler() override = default;
    virtual void handle(const ::jxx::Ptr<HttpExchange>& exchange) = 0;
};
}
