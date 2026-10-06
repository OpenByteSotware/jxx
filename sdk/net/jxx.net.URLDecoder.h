#pragma once

#include "lang/jxx.lang.ClassInfo.h"

#include "lang/jxx_types.h"
#include "lang/jxx.lang.String.h"

namespace jxx::net
{
    class URLDecoder final : public ::jxx::lang::ClassBase<URLDecoder, ::jxx::lang::Object> {
    public:
        static jxx::Ptr<jxx::lang::String> decode(const jxx::Ptr<jxx::lang::String>& s);
        static jxx::Ptr<jxx::lang::String> decode(const jxx::Ptr<jxx::lang::String>& s,
                                                  const jxx::Ptr<jxx::lang::String>& enc);
    };
}
