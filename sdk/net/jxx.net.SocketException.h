#pragma once

#include <string>

#include "io/jxx.io.IOException.h"
#include "lang/jxx.lang.ClassInfo.h"
#include "lang/jxx.lang.ClassInfoMarker.h"

namespace jxx::net {

class SocketException
    : public ::jxx::lang::ClassBase<
          SocketException,
          ::jxx::io::IOException> {
public:
    using JxxSuper = ::jxx::io::IOException;
    using Super = ::jxx::lang::ClassBase<
        SocketException,
        JxxSuper>;
    using JxxClassInfoMarker =
        ::jxx::lang::ClassInfo<
            SocketException,
            JxxSuper>;

    static ::jxx::Ptr<
        ::jxx::lang::ClassAny>
    Class();

public:
    SocketException();

    explicit SocketException(
        const ::jxx::Ptr<
            ::jxx::lang::String>&
                message);

    explicit SocketException(
        const char* message);

    explicit SocketException(
        const std::string& message);

    SocketException(
        const SocketException&) =
            default;

    SocketException(
        SocketException&&)
        noexcept = default;

    SocketException& operator=(
        const SocketException&) =
            default;

    SocketException& operator=(
        SocketException&&)
        noexcept = default;

    ~SocketException()
        override = default;

protected:
    ::jxx::Ptr<::jxx::lang::Object>
    cloneImpl() const override;

    const char* typeName()
        const noexcept override;
};

} // namespace jxx::net
