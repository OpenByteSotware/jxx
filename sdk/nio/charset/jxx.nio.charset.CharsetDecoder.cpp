#include "nio/charset/jxx.nio.charset.CharsetDecoder.h"
#include "nio/charset/jxx.nio.charset.Charset.h"

#include <stdexcept>

namespace
{
    [[noreturn]] void throwIAE_(const char* msg)
    {
        throw std::invalid_argument(msg);
    }
}

namespace jxx::nio::charset
{
    CharsetDecoder::CharsetDecoder(jxx::Ptr<Charset> cs)
        : cs_(std::move(cs)),
          malformedAction_(CodingErrorAction::REPORT),
          unmappableAction_(CodingErrorAction::REPORT),
          replacement_(jxx::NEW<jxx::lang::String>("?"))
    {
        if (!cs_)
            throwIAE_("null charset");
    }

    jxx::Ptr<CharsetDecoder> CharsetDecoder::self_()
    {
		return jxx::CAST<CharsetDecoder, jxx::lang::Object>(this->thisPtr);
    }

    jxx::Ptr<Charset> CharsetDecoder::charset() const { return cs_; }
    jxx::Ptr<CharsetDecoder> CharsetDecoder::onMalformedInput(jxx::Ptr<CodingErrorAction> action) { malformedAction_ = action; return self_(); }
    jxx::Ptr<CharsetDecoder> CharsetDecoder::onUnmappableCharacter(jxx::Ptr<CodingErrorAction> action) { unmappableAction_ = action; return self_(); }
    jxx::Ptr<CharsetDecoder> CharsetDecoder::replaceWith(jxx::Ptr<jxx::lang::String> newReplacement) { if (newReplacement) replacement_ = newReplacement; return self_(); }
    jxx::Ptr<jxx::lang::String> CharsetDecoder::replacement() const { return replacement_; }

    jxx::Ptr<jxx::nio::CharBuffer> CharsetDecoder::decode(jxx::Ptr<jxx::nio::ByteBuffer> in)
    {
        if (!in)
            throwIAE_("null input buffer");
        auto bytes = jxx::NEW<jxx::lang::ByteArrayType>(in->remaining());
        in->get(bytes, 0, bytes->length);
        auto str = jxx::NEW<jxx::lang::String>(bytes, cs_->name());
        return jxx::nio::CharBuffer::wrap(str);
    }

    jxx::Ptr<CoderResult> CharsetDecoder::decode(jxx::Ptr<jxx::nio::ByteBuffer> in,
                                                 jxx::Ptr<jxx::nio::CharBuffer> out,
                                                 jxx::lang::jbool /*endOfInput*/)
    {
        if (!in || !out)
            throwIAE_("null buffer");
        auto tmp = decode(in);
        if (out->remaining() < tmp->remaining())
            return CoderResult::JXX_OVERFLOW;
        out->put(tmp);
        return CoderResult::JXX_UNDERFLOW;
    }

    jxx::Ptr<CoderResult> CharsetDecoder::flush(jxx::Ptr<jxx::nio::CharBuffer> /*out*/)
    {
        return CoderResult::JXX_UNDERFLOW;
    }

    jxx::Ptr<CharsetDecoder> CharsetDecoder::reset() { return self_(); }
}
