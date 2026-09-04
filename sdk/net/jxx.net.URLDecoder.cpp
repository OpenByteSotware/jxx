#include "net/jxx.net.URLDecoder.h"

#include <stdexcept>

#include "net/internal/jxx.net.internal.UrlUtil.h"

namespace jxx::net
{
    jxx::Ptr<jxx::lang::String> URLDecoder::decode(const jxx::Ptr<jxx::lang::String>& s)
    {
        return decode(s, jxx::NEW<jxx::lang::String>("UTF-8"));
    }

    jxx::Ptr<jxx::lang::String> URLDecoder::decode(const jxx::Ptr<jxx::lang::String>& s,
                                                   const jxx::Ptr<jxx::lang::String>& /*enc*/)
    {
        if (!s)
            throw std::invalid_argument("null input");
        return jxx::NEW<jxx::lang::String>(internal::percentDecodeForm(s->utf8()));
    }
}
