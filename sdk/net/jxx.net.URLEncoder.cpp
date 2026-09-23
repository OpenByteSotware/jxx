#include "net/jxx.net.URLEncoder.h"


#include "net/internal/jxx.net.internal.UrlUtil.h"
#include "io/jxx.io.UnsupportedEncodingException.h"
#include "lang/jxx.lang.NullPointerException.h"
#include "nio/charset/jxx.nio.charset.Charset.h"

namespace jxx::net
{
    jxx::Ptr<jxx::lang::String> URLEncoder::encode(const jxx::Ptr<jxx::lang::String>& s)
    {
        return encode(std::move(s), jxx::NEW<jxx::lang::String>("UTF-8"));
    }

    jxx::Ptr<jxx::lang::String> URLEncoder::encode(const jxx::Ptr<jxx::lang::String>& s,
                                                   const jxx::Ptr<jxx::lang::String>& enc)
    {
        if (s == nullptr || enc == nullptr) {
            throw jxx::lang::NullPointerException();
        }
        if (!jxx::nio::charset::Charset::isSupported(enc)) {
            throw jxx::io::UnsupportedEncodingException(enc);
        }
        return jxx::NEW<jxx::lang::String>(internal::percentEncodeForm(s->utf8()));
    }
}
