#include "net/jxx.net.URLEncoder.h"

#include <stdexcept>

#include "net/internal/jxx.net.internal.UrlUtil.h"

namespace jxx::net
{
    jxx::Ptr<jxx::lang::String> URLEncoder::encode(jxx::Ptr<jxx::lang::String> s)
    {
        return encode(std::move(s), jxx::NEW<jxx::lang::String>("UTF-8"));
    }

    jxx::Ptr<jxx::lang::String> URLEncoder::encode(jxx::Ptr<jxx::lang::String> s,
                                                   jxx::Ptr<jxx::lang::String> /*enc*/)
    {
        if (!s)
            throw std::invalid_argument("null input");
        return jxx::NEW<jxx::lang::String>(internal::percentEncodeForm(s->utf8()));
    }
}
