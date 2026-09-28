#pragma once

#include "lang/jxx.lang.Object.h"
#include "lang/jxx.lang.buildin_array.h"
#include "lang/jxx_types.h"

namespace jxx::com::sun::net::httpserver::internal {

class RequestBodySource
    : public ::jxx::lang::ClassBase<RequestBodySource, ::jxx::lang::Object> {
public:
    using JxxSuper = ::jxx::lang::Object;
    using Super = ::jxx::lang::ClassBase<RequestBodySource, JxxSuper>;
    ~RequestBodySource() override = default;

    virtual ::jxx::lang::jint read(
        const ::jxx::lang::ByteArray& buffer,
        ::jxx::lang::jint offset,
        ::jxx::lang::jint length) = 0;
    virtual ::jxx::lang::jlong skip(::jxx::lang::jlong count) = 0;
    virtual ::jxx::lang::jint available() = 0;
    virtual void close() = 0;
    virtual ::jxx::lang::jbool isFullyConsumedInternal() const noexcept = 0;
    virtual ::jxx::lang::jbool wasClosedInternal() const noexcept = 0;

protected:
    RequestBodySource() = default;
};

} // namespace jxx::com::sun::net::httpserver::internal
