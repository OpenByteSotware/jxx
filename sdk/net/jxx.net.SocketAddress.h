#pragma once

#include "jxx_types.h"
#include "jxx.lang.Object.h"

namespace jxx::net
{
    class SocketAddress : public jxx::lang::Object
    {
    public:
        ~SocketAddress() override = default;
    };
}
