#pragma once
#include "lang/jxx.lang.ClassInfo.h"

#include "lang/jxx_types.h"

namespace jxx::net
{
    class SocketImpl;

    class SocketImplFactory : public jxx::lang::InterfaceBase<SocketImplFactory> {
    public:
        virtual ~SocketImplFactory() = default;

    public:
        virtual jxx::Ptr<SocketImpl> createSocketImpl() = 0;
    };
}
