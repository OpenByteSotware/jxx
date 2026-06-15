#pragma once

#include "lang/jxx_types.h"
#include "lang/jxx.lang.String.h"

namespace jxx::net
{
    class IDN final
    {
    public:
        static constexpr jxx::lang::jint ALLOW_UNASSIGNED = 0x01;
        static constexpr jxx::lang::jint USE_STD3_ASCII_RULES = 0x02;

    public:
        static jxx::Ptr<jxx::lang::String> toASCII(jxx::Ptr<jxx::lang::String> input);
        static jxx::Ptr<jxx::lang::String> toASCII(jxx::Ptr<jxx::lang::String> input,
                                                   jxx::lang::jint flags);
        static jxx::Ptr<jxx::lang::String> toUnicode(jxx::Ptr<jxx::lang::String> input);
        static jxx::Ptr<jxx::lang::String> toUnicode(jxx::Ptr<jxx::lang::String> input,
                                                     jxx::lang::jint flags);
    };
}
