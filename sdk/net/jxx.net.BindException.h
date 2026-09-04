#pragma once
#include "lang/jxx.lang.ClassInfo.h"

#include "net/jxx.net.SocketException.h"

namespace jxx::net
{
    class BindException : public jxx::lang::ClassBase<BindException, SocketException> {
public:
    using JavaSuper = SocketException;
    using Super = jxx::lang::ClassBase<BindException, SocketException>;

    public:
        BindException();
        explicit BindException(const char* message);
        explicit BindException(const std::string& message);
        ~BindException() override = default;
    };
}
