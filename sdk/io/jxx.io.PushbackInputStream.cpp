#include "io/jxx.io.PushbackInputStream.h"

#include <algorithm>
#include <stdexcept>

namespace
{
    void throwIAE_(const char* msg)
    {
        throw std::invalid_argument(msg);
    }

    void throwIOE_(const char* msg)
    {
        throw std::runtime_error(msg);
    }
}

namespace jxx::io
{
    PushbackInputStream::PushbackInputStream(jxx::Ptr<InputStream> in)
        : PushbackInputStream(std::move(in), 1)
    {
    }

    PushbackInputStream::PushbackInputStream(jxx::Ptr<InputStream> in, jxx::lang::jint size)
        : FilterInputStream(std::move(in))
    {
        if (size <= 0)
            throwIAE_("size <= 0");

        buf_ = jxx::NEW<jxx::JxxArray<jxx::lang::jbyte, 1U>>(size);
        pos_ = size;
    }

    jxx::lang::jint PushbackInputStream::read()
    {
        if (pos_ < buf_->length)
            return (*buf_)[pos_++] & 0xFF;
        return in_->read();
    }

    jxx::lang::jint PushbackInputStream::read(const jxx::lang::ByteArray b,
                                              jxx::lang::jint off,
                                              jxx::lang::jint len)
    {
        if (!b)
            throwIAE_("null byte array");
        if (off < 0 || len < 0 || off > b->length - len)
            throwIAE_("index out of bounds");

        const jxx::lang::jint avail = buf_->length - pos_;
        if (avail > 0)
        {
            const jxx::lang::jint copyLen = std::min(len, avail);
            for (jxx::lang::jint i = 0; i < copyLen; ++i)
                (*b)[off + i] = (*buf_)[pos_ + i];

            pos_ += copyLen;
            if (copyLen == len)
                return len;

            const jxx::lang::jint n = in_->read(b, off + copyLen, len - copyLen);
            return (n < 0) ? copyLen : copyLen + n;
        }

        return in_->read(b, off, len);
    }

    void PushbackInputStream::unread(jxx::lang::jint b)
    {
        if (pos_ == 0)
            throwIOE_("push back buffer is full");
        (*buf_)[--pos_] = static_cast<jxx::lang::jbyte>(b);
    }

    void PushbackInputStream::unread(const jxx::lang::ByteArray b)
    {
        if (!b)
            throwIAE_("null byte array");
        unread(b, 0, b->length);
    }

    void PushbackInputStream::unread(const jxx::lang::ByteArray b,
                                     jxx::lang::jint off,
                                     jxx::lang::jint len)
    {
        if (!b)
            throwIAE_("null byte array");
        if (off < 0 || len < 0 || off > b->length - len)
            throwIAE_("index out of bounds");
        if (len > pos_)
            throwIOE_("push back buffer is full");

        for (jxx::lang::jint i = len - 1; i >= 0; --i)
            (*buf_)[--pos_] = (*b)[off + i];
    }

    jxx::lang::jint PushbackInputStream::available()
    {
        return (buf_->length - pos_) + in_->available();
    }

    jxx::lang::jlong PushbackInputStream::skip(jxx::lang::jlong n)
    {
        if (n <= 0)
            return 0;

        const jxx::lang::jint avail = buf_->length - pos_;
        if (avail > 0)
        {
            const jxx::lang::jint skipped = static_cast<jxx::lang::jint>(std::min<jxx::lang::jlong>(n, avail));
            pos_ += skipped;
            return skipped;
        }

        return in_->skip(n);
    }

    jxx::lang::jbool PushbackInputStream::markSupported() const { return false; }
}
