#pragma once

#include <string>

#include "io/jxx.io.InterruptedIOException.h"
#include "lang/jxx.lang.ClassInfo.h"
#include "lang/jxx.lang.ClassInfoMarker.h"

namespace jxx::net {

class SocketTimeoutException
    : public ::jxx::lang::ClassBase<
          SocketTimeoutException,
          ::jxx::io::InterruptedIOException> {
public:
    using JxxSuper = ::jxx::io::InterruptedIOException;
    using Super = ::jxx::lang::ClassBase<
        SocketTimeoutException,
        JxxSuper>;
    using JxxClassInfoMarker =
        ::jxx::lang::ClassInfo<
            SocketTimeoutException,
            JxxSuper>;

    static ::jxx::Ptr<
        ::jxx::lang::ClassAny>
    Class();

public:
    SocketTimeoutException();

    explicit SocketTimeoutException(
        const ::jxx::Ptr<
            ::jxx::lang::String>&
                message);

    explicit SocketTimeoutException(
        const char* message);

    explicit SocketTimeoutException(
        const std::string& message);

    SocketTimeoutException(
        const SocketTimeoutException&) =
            default;

    SocketTimeoutException(
        SocketTimeoutException&&)
        noexcept = default;

    SocketTimeoutException& operator=(
        const SocketTimeoutException&) =
            default;

    SocketTimeoutException& operator=(
        SocketTimeoutException&&)
        noexcept = default;

    ~SocketTimeoutException()
        override = default;

protected:
    ::jxx::Ptr<::jxx::lang::Object>
    cloneImpl() const override;

    const char* typeName()
        const noexcept override;
};

} // namespace jxx::net
