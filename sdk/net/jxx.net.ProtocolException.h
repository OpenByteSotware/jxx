#pragma once

#include <string>

#include "io/jxx.io.IOException.h"
#include "lang/jxx.lang.ClassInfo.h"
#include "lang/jxx.lang.String.h"

namespace jxx::net {

class ProtocolException
    : public ::jxx::lang::ClassBase<
          ProtocolException,
          ::jxx::io::IOException> {
public:
    using JxxSuper = ::jxx::io::IOException;
    using Super =
        ::jxx::lang::ClassBase<ProtocolException, JxxSuper>;
    using JxxClassInfoMarker =
        ::jxx::lang::ClassInfo<ProtocolException, JxxSuper>;

    static ::jxx::Ptr<::jxx::lang::ClassAny> Class();

    ProtocolException();

    explicit ProtocolException(
        const ::jxx::Ptr<::jxx::lang::String>& message);

    explicit ProtocolException(const char* message);
    explicit ProtocolException(const std::string& message);

    ProtocolException(const ProtocolException&) = default;
    ProtocolException(ProtocolException&&) noexcept = default;
    ProtocolException& operator=(const ProtocolException&) = default;
    ProtocolException& operator=(ProtocolException&&) noexcept = default;
    ~ProtocolException() override = default;

protected:
    ::jxx::Ptr<::jxx::lang::Object> cloneImpl() const override;
    const char* typeName() const noexcept override;
};

} // namespace jxx::net
