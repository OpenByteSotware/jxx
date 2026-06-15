#pragma once

#include "lang/jxx_types.h"
#include "lang/jxx.lang.Object.h"

namespace jxx::net
{
    class SocketAddress : public jxx::lang::Object
    {
    public:
        ~SocketAddress() override = default;
    };
}
