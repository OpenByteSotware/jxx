#pragma once
#include "lang/jxx.lang.ClassInfo.h"

#include "lang/jxx_types.h"
#include "lang/jxx.lang.Object.h"

namespace jxx::net
{
    class Proxy : public jxx::lang::ClassBase<Proxy, jxx::lang::Object> {
public:
    using JavaSuper = jxx::lang::Object;
    using Super = jxx::lang::ClassBase<Proxy, jxx::lang::Object>;

    public:
        ~Proxy() override = default;
    };
}
