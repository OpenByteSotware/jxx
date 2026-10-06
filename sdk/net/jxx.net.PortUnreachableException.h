#pragma once

#include "lang/jxx.lang.ClassInfo.h"

#include "net/jxx.net.SocketException.h"

namespace jxx::net
{
    class PortUnreachableException :
        public ::jxx::lang::ClassBase<PortUnreachableException, SocketException> {
    public:
        PortUnreachableException();
        explicit PortUnreachableException(const char* message);
        explicit PortUnreachableException(const std::string& message);
        ~PortUnreachableException() override = default;
    };
}
