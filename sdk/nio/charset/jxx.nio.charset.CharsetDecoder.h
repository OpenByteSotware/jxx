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

    class CharsetDecoder final : public jxx::lang::Object
    {
    public:
        explicit CharsetDecoder(jxx::Ptr<Charset> cs);
        ~CharsetDecoder() override = default;

    public:
        jxx::Ptr<Charset> charset() const;
        jxx::Ptr<CharsetDecoder> onMalformedInput(jxx::Ptr<CodingErrorAction> action);
        jxx::Ptr<CharsetDecoder> onUnmappableCharacter(jxx::Ptr<CodingErrorAction> action);
        jxx::Ptr<CharsetDecoder> replaceWith(jxx::Ptr<jxx::lang::String> newReplacement);
        jxx::Ptr<jxx::lang::String> replacement() const;

        jxx::Ptr<jxx::nio::CharBuffer> decode(jxx::Ptr<jxx::nio::ByteBuffer> in);
        jxx::Ptr<CoderResult> decode(jxx::Ptr<jxx::nio::ByteBuffer> in,
                                     jxx::Ptr<jxx::nio::CharBuffer> out,
                                     jxx::lang::jbool endOfInput);
        jxx::Ptr<CoderResult> flush(jxx::Ptr<jxx::nio::CharBuffer> out);
        jxx::Ptr<CharsetDecoder> reset();

    private:
        jxx::Ptr<CharsetDecoder> self_();

    private:
        jxx::Ptr<Charset> cs_;
        jxx::Ptr<CodingErrorAction> malformedAction_;
        jxx::Ptr<CodingErrorAction> unmappableAction_;
        jxx::Ptr<jxx::lang::String> replacement_;
    };
}
