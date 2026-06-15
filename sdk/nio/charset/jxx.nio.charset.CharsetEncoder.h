#pragma once

#include "lang/jxx_types.h"
#include "lang/jxx.lang.String.h"
#include "nio/jxx.nio.ByteBuffer.h"
#include "nio/jxx.nio.CharBuffer.h"
#include "nio/charset/jxx.nio.charset.CoderResult.h"
#include "nio/charset/jxx.nio.charset.CodingErrorAction.h"

namespace jxx::nio::charset
{
    class Charset;

    class CharsetEncoder final : public jxx::lang::Object
    {
    public:
        explicit CharsetEncoder(jxx::Ptr<Charset> cs);
        ~CharsetEncoder() override = default;

    public:
        jxx::Ptr<Charset> charset() const;
        jxx::Ptr<CharsetEncoder> onMalformedInput(jxx::Ptr<CodingErrorAction> action);
        jxx::Ptr<CharsetEncoder> onUnmappableCharacter(jxx::Ptr<CodingErrorAction> action);
        jxx::Ptr<CharsetEncoder> replaceWith(const jxx::lang::ByteArray replacement);
        jxx::lang::ByteArray replacement() const;

        jxx::Ptr<jxx::nio::ByteBuffer> encode(jxx::Ptr<jxx::nio::CharBuffer> in);
        jxx::Ptr<CoderResult> encode(jxx::Ptr<jxx::nio::CharBuffer> in,
                                     jxx::Ptr<jxx::nio::ByteBuffer> out,
                                     jxx::lang::jbool endOfInput);
        jxx::Ptr<CoderResult> flush(jxx::Ptr<jxx::nio::ByteBuffer> out);
        jxx::Ptr<CharsetEncoder> reset();

    private:
        jxx::Ptr<CharsetEncoder> self_();

    private:
        jxx::Ptr<Charset> cs_;
        jxx::Ptr<CodingErrorAction> malformedAction_;
        jxx::Ptr<CodingErrorAction> unmappableAction_;
        jxx::lang::ByteArray replacement_;
    };
}
