#pragma once
#include "lang/jxx.lang.ClassInfo.h"

#include "net/jxx.net.SocketException.h"

namespace jxx::net
{
    class BindException : public jxx::lang::ClassBase<BindException, SocketException> {
public:
    using JxxSuper = SocketException;
    using Super = jxx::lang::ClassBase<BindException, SocketException>;

    public:
        BindException();
        explicit BindException(const char* message);
        explicit BindException(const std::string& message);
        ~BindException() override = default;
    };
}
