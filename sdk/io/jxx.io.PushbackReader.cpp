#include <algorithm>
#include <stdexcept>
#include "io/jxx.io.PushbackReader.h"


namespace
{
    [[noreturn]] void throwIAE_(const char* msg)
    {
        throw std::invalid_argument(msg);
    }

    [[noreturn]] void throwIOE_(const char* msg)
    {
        throw std::runtime_error(msg);
    }
}

namespace jxx::io
{
    PushbackReader::PushbackReader(jxx::Ptr<Reader> in)
        : PushbackReader(std::move(in), 1)
    {
    }

    PushbackReader::PushbackReader(jxx::Ptr<Reader> in, jxx::lang::jint size)
        : FilterReader(std::move(in))
    {
        if (size <= 0)
            throwIAE_("size <= 0");
        buf_ = jxx::NEW<jxx::lang::CharArrayType>(size);
        pos_ = size;
    }

    jxx::lang::jint PushbackReader::read()
    {
        if (closed_)
            throwIOE_("reader closed");
        if (pos_ < buf_->length)
            return static_cast<jxx::lang::jint>((*buf_)[pos_++]);
        return in_->read();
    }

    jxx::lang::jint PushbackReader::read(const jxx::lang::CharArray cbuf,
                                         jxx::lang::jint off,
                                         jxx::lang::jint len)
    {
        if (closed_)
            throwIOE_("reader closed");
        if (!cbuf)
            throwIAE_("null char array");
        if (off < 0 || len < 0 || off > cbuf->length - len)
            throwIAE_("index out of bounds");

        const jxx::lang::jint avail = buf_->length - pos_;
        if (avail > 0)
        {
            const auto copyLen = std::min(len, avail);
            for (jxx::lang::jint i = 0; i < copyLen; ++i)
                (*cbuf)[off + i] = (*buf_)[pos_ + i];
            pos_ += copyLen;
            if (copyLen == len)
                return len;
            const auto n = in_->read(cbuf, off + copyLen, len - copyLen);
            return n < 0 ? copyLen : copyLen + n;
        }

        return in_->read(cbuf, off, len);
    }

    void PushbackReader::unread(jxx::lang::jint c)
    {
        if (closed_)
            throwIOE_("reader closed");
        if (pos_ == 0)
            throwIOE_("pushback buffer full");
        (*buf_)[--pos_] = static_cast<jxx::lang::jchar>(c);
    }

    void PushbackReader::unread(const jxx::lang::CharArray cbuf)
    {
        if (!cbuf)
            throwIAE_("null char array");
        unread(cbuf, 0, cbuf->length);
    }

    void PushbackReader::unread(const jxx::lang::CharArray cbuf,
                                jxx::lang::jint off,
                                jxx::lang::jint len)
    {
        if (closed_)
            throwIOE_("reader closed");
        if (!cbuf)
            throwIAE_("null char array");
        if (off < 0 || len < 0 || off > cbuf->length - len)
            throwIAE_("index out of bounds");
        if (len > pos_)
            throwIOE_("pushback buffer full");

        for (jxx::lang::jint i = len - 1; i >= 0; --i)
            (*buf_)[--pos_] = (*cbuf)[off + i];
    }

    jxx::lang::jbool PushbackReader::ready()
    {
        if (closed_)
            throwIOE_("reader closed");
        return (pos_ < buf_->length) || in_->ready();
    }

    jxx::lang::jbool PushbackReader::markSupported() const { return false; }

    jxx::lang::jlong PushbackReader::skip(jxx::lang::jlong n)
    {
        if (closed_)
            throwIOE_("reader closed");
        if (n < 0)
            throwIAE_("skip value is negative");

        const auto avail = buf_->length - pos_;
        if (avail > 0)
        {
            const auto skipped = static_cast<jxx::lang::jint>(std::min<jxx::lang::jlong>(n, avail));
            pos_ += skipped;
            return skipped;
        }
        return in_->skip(n);
    }

    void PushbackReader::close()
    {
        if (!closed_)
        {
            closed_ = true;
            buf_ = nullptr;
            in_->close();
        }
    }
}
