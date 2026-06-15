#pragma once

#include "jxx_types.h"

namespace jxx::net
{
    class SocketImpl;

    class SocketImplFactory
    {
    public:
        virtual ~SocketImplFactory() = default;

    public:
        virtual jxx::Ptr<SocketImpl> createSocketImpl() = 0;
    };
}
