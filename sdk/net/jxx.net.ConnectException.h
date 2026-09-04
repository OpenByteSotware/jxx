#pragma once
#include "lang/jxx.lang.ClassInfo.h"

#include "net/jxx.net.SocketException.h"

namespace jxx::net
{
    class ConnectException : public jxx::lang::ClassBase<ConnectException, SocketException> {
public:
    using JavaSuper = SocketException;
    using Super = jxx::lang::ClassBase<ConnectException, SocketException>;

    public:
        ConnectException();
        explicit ConnectException(const char* message);
        explicit ConnectException(const std::string& message);
        ~ConnectException() override = default;
    };
}
