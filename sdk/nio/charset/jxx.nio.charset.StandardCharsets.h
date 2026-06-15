#pragma once

#include "lang/jxx_types.h"
#include "nio/charset/jxx.nio.charset.Charset.h"

namespace jxx::nio::charset
{
    class StandardCharsets final
    {
    public:
        static jxx::Ptr<Charset> US_ASCII;
        static jxx::Ptr<Charset> ISO_8859_1;
        static jxx::Ptr<Charset> UTF_8;
        static jxx::Ptr<Charset> UTF_16BE;
        static jxx::Ptr<Charset> UTF_16LE;
        static jxx::Ptr<Charset> UTF_16;
    };
}
