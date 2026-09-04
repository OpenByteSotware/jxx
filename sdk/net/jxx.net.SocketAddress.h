#pragma once
#include "lang/jxx.lang.ClassInfo.h"

#include "lang/jxx_types.h"
#include "lang/jxx.lang.Object.h"

namespace jxx::net
{
    class SocketAddress : public jxx::lang::ClassBase<SocketAddress, jxx::lang::Object> {
public:
    using JavaSuper = jxx::lang::Object;
    using Super = jxx::lang::ClassBase<SocketAddress, jxx::lang::Object>;

    public:
        ~SocketAddress() override = default;
    };
}
