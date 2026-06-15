#pragma once

#include "lang/jxx_types.h"
#include "lang/jxx.lang.Object.h"
#include "io/jxx.io.Closeable.h"

namespace jxx::nio::channels
{
    class Channel
        : public jxx::lang::Object
        , public jxx::io::Closeable
    {
    public:
        ~Channel() override = default;

    public:
        virtual jxx::lang::jbool isOpen() const = 0;
        virtual void close() override = 0;
    };
}
