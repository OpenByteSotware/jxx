#include <stdexcept>
#include "nio/charset/jxx.nio.charset.Charset.h"
#include "nio/charset/jxx.nio.charset.CharsetEncoder.h"

namespace
{
    [[noreturn]] void throwIAE_(const char* msg)
    {
        throw std::invalid_argument(msg);
    }
}

namespace jxx::nio::charset
{
    CharsetEncoder::CharsetEncoder(jxx::Ptr<Charset> cs)
        : cs_(std::move(cs)),
          malformedAction_(CodingErrorAction::REPORT),
          unmappableAction_(CodingErrorAction::REPORT),
          replacement_(std::make_shared<jxx::JxxArray<jxx::lang::jbyte, 1U>>(1))
    {
        if (!cs_)
            throwIAE_("null charset");
        (*replacement_)[0] = static_cast<jxx::lang::jbyte>('?');
    }

    jxx::Ptr<CharsetEncoder> CharsetEncoder::self_()
    {
        if (thisPtr)
            return std::static_pointer_cast<CharsetEncoder>(thisPtr);
        return std::static_pointer_cast<CharsetEncoder>(shared_from_this());
    }

    jxx::Ptr<Charset> CharsetEncoder::charset() const { return cs_; }
    jxx::Ptr<CharsetEncoder> CharsetEncoder::onMalformedInput(jxx::Ptr<CodingErrorAction> action) { malformedAction_ = action; return self_(); }
    jxx::Ptr<CharsetEncoder> CharsetEncoder::onUnmappableCharacter(jxx::Ptr<CodingErrorAction> action) { unmappableAction_ = action; return self_(); }
    jxx::Ptr<CharsetEncoder> CharsetEncoder::replaceWith(const jxx::lang::ByteArray replacement) { if (replacement) replacement_ = replacement; return self_(); }
    jxx::lang::ByteArray CharsetEncoder::replacement() const { return replacement_; }

    jxx::Ptr<jxx::nio::ByteBuffer> CharsetEncoder::encode(jxx::Ptr<jxx::nio::CharBuffer> in)
    {
        if (!in)
            throwIAE_("null input buffer");
        auto chars = std::make_shared<jxx::JxxArray<jxx::lang::jchar, 1U>>(in->remaining());
        in->get(chars, 0, chars->length());
        auto str = std::make_shared<jxx::lang::String>(chars);
        auto bytes = str->getBytes(cs_->name());
        return jxx::nio::ByteBuffer::wrap(bytes);
    }

    jxx::Ptr<CoderResult> CharsetEncoder::encode(jxx::Ptr<jxx::nio::CharBuffer> in,
                                                 jxx::Ptr<jxx::nio::ByteBuffer> out,
                                                 jxx::lang::jbool /*endOfInput*/)
    {
        if (!in || !out)
            throwIAE_("null buffer");
        auto tmp = encode(in);
        if (out->remaining() < tmp->remaining())
            return CoderResult::JXX_OVERFLOW;
        out->put(tmp);
        return CoderResult::JXX_UNDERFLOW;
    }

    jxx::Ptr<CoderResult> CharsetEncoder::flush(jxx::Ptr<jxx::nio::ByteBuffer> /*out*/)
    {
        return CoderResult::JXX_UNDERFLOW;
    }

    jxx::Ptr<CharsetEncoder> CharsetEncoder::reset() { return self_(); }
}
