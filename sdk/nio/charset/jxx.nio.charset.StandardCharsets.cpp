#include "lang/jxx.lang.String.h"
#include "nio/charset/jxx.nio.charset.StandardCharsets.h"

namespace jxx::nio::charset
{
    jxx::Ptr<Charset> StandardCharsets::US_ASCII = Charset::forName(jxx::NEW<jxx::lang::String>("US-ASCII"));
    jxx::Ptr<Charset> StandardCharsets::ISO_8859_1 = Charset::forName(jxx::NEW<jxx::lang::String>("ISO-8859-1"));
    jxx::Ptr<Charset> StandardCharsets::UTF_8 = Charset::forName(jxx::NEW<jxx::lang::String>("UTF-8"));
    jxx::Ptr<Charset> StandardCharsets::UTF_16BE = Charset::forName(jxx::NEW<jxx::lang::String>("UTF-16BE"));
    jxx::Ptr<Charset> StandardCharsets::UTF_16LE = Charset::forName(jxx::NEW<jxx::lang::String>("UTF-16LE"));
    jxx::Ptr<Charset> StandardCharsets::UTF_16 = Charset::forName(jxx::NEW<jxx::lang::String>("UTF-16"));
}
