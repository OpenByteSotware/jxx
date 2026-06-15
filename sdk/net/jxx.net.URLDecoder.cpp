#include "net/jxx.net.URLDecoder.h"

#include <stdexcept>

#include "net/internal/jxx.net.internal.UrlUtil.h"

namespace jxx::net
{
    jxx::Ptr<jxx::lang::String> URLDecoder::decode(jxx::Ptr<jxx::lang::String> s)
    {
        return decode(std::move(s), std::make_shared<jxx::lang::String>("UTF-8"));
    }

    jxx::Ptr<jxx::lang::String> URLDecoder::decode(jxx::Ptr<jxx::lang::String> s,
                                                   jxx::Ptr<jxx::lang::String> /*enc*/)
    {
        if (!s)
            throw std::invalid_argument("null input");
        return std::make_shared<jxx::lang::String>(internal::percentDecodeForm(s->utf8()));
    }
}
