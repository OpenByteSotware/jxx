#pragma once

#include "lang/jxx_types.h"
#include "lang/jxx.lang.String.h"

namespace jxx::net
{
    class URLEncoder final
    {
    public:
        static jxx::Ptr<jxx::lang::String> encode(jxx::Ptr<jxx::lang::String> s);
        static jxx::Ptr<jxx::lang::String> encode(jxx::Ptr<jxx::lang::String> s,
                                                  jxx::Ptr<jxx::lang::String> enc);
    };
}
