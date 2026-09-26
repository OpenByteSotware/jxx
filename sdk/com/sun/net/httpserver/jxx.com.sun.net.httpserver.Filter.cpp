#include "com/sun/net/httpserver/jxx.com.sun.net.httpserver.Filter.h"
#include "com/sun/net/httpserver/jxx.com.sun.net.httpserver.HttpExchange.h"
#include "com/sun/net/httpserver/jxx.com.sun.net.httpserver.HttpHandler.h"
#include "lang/jxx.lang.Exceptions.h"
#include "util/jxx.util.List.h"

namespace jxx::com::sun::net::httpserver {

Filter::Chain::Chain(
    const ::jxx::Ptr<::jxx::util::List<Filter>>& filters,
    const ::jxx::Ptr<HttpHandler>& handler)
    : Super(), filters_(filters), handler_(handler)
{
    if (filters_ == nullptr || handler_ == nullptr) {
        throw ::jxx::lang::NullPointerException();
    }
}

void Filter::Chain::doFilter(
    const ::jxx::Ptr<HttpExchange>& exchange)
{
    if (exchange == nullptr) {
        throw ::jxx::lang::NullPointerException();
    }

    if (index_ < filters_->size()) {
        auto filter = filters_->get(index_++);
        if (filter == nullptr) {
            throw ::jxx::lang::NullPointerException();
        }
        filter->doFilter(
            exchange,
            ::jxx::CAST<Filter::Chain>(this->thisPtr()));
        return;
    }

    handler_->handle(exchange);
}

} // namespace jxx::com::sun::net::httpserver
