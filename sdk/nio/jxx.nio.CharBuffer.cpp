#include "nio/jxx.nio.CharBuffer.h"

#include <stdexcept>

namespace
{
    [[noreturn]] void throwIAE_(const char* msg)
    {
        throw std::invalid_argument(msg);
    }

    [[noreturn]] void throwState_(const char* msg)
    {
        throw std::runtime_error(msg);
    }
}

namespace jxx::nio
{
    CharBuffer::CharBuffer(std::shared_ptr<std::vector<jxx::lang::jchar>> storage,
                 jxx::lang::jint offset,
                 jxx::lang::jint capacity,
                 jxx::lang::jbool readOnly)
        : Buffer(-1, 0, capacity, capacity),
          storage_(std::move(storage)),
          offset_(offset),
          readOnly_(readOnly)
    {
        if (!storage_)
            throwIAE_("null storage");
    }

    jxx::Ptr<CharBuffer> CharBuffer::allocate(jxx::lang::jint capacity)
    {
        if (capacity < 0)
            throwIAE_("capacity < 0");
        auto storage = std::make_shared<std::vector<jxx::lang::jchar>>(static_cast<std::size_t>(capacity));
        return std::shared_ptr<CharBuffer>(new CharBuffer(storage, 0, capacity, false));
    }

    jxx::Ptr<CharBuffer> CharBuffer::wrap(const jxx::lang::CharArray array)
    {
        if (!array)
            throwIAE_("null array");
        return wrap(array, 0, array->length);
    }

    jxx::Ptr<CharBuffer> CharBuffer::wrap(const jxx::lang::CharArray array,
                                jxx::lang::jint offset,
                                jxx::lang::jint length)
    {
        if (!array)
            throwIAE_("null array");
        if (offset < 0 || length < 0 || offset > array->length - length)
            throwIAE_("index out of bounds");

        auto storage = std::make_shared<std::vector<jxx::lang::jchar>>(static_cast<std::size_t>(array->length));
        for (jxx::lang::jint i = 0; i < array->length; ++i)
            (*storage)[static_cast<std::size_t>(i)] = (*array)[i];

        auto out = std::shared_ptr<CharBuffer>(new CharBuffer(storage, offset, length, false));
        out->position(0);
        out->limit(length);
        return out;
    }
    jxx::Ptr<CharBuffer> CharBuffer::wrap(jxx::Ptr<jxx::lang::String> str)
    {
        if (!str)
            throwIAE_("null string");
        return wrap(str->toCharArray(), 0, str->length());
    }


    jxx::Ptr<CharBuffer> CharBuffer::self__()
    {
        if (thisPtr)
            return std::static_pointer_cast<CharBuffer>(thisPtr);
        return std::static_pointer_cast<CharBuffer>(shared_from_this());
    }

    void CharBuffer::checkWritable_() const
    {
        if (readOnly_)
            throwState_("buffer is read-only");
    }

    jxx::lang::jchar& CharBuffer::at_(jxx::lang::jint index)
    {
        return (*storage_)[static_cast<std::size_t>(offset_ + index)];
    }

    const jxx::lang::jchar& CharBuffer::at_(jxx::lang::jint index) const
    {
        return (*storage_)[static_cast<std::size_t>(offset_ + index)];
    }

    jxx::lang::jchar CharBuffer::get()
    {
        checkRemaining_(1);
        return at_(pos_++);
    }

    jxx::lang::jchar CharBuffer::get(jxx::lang::jint index) const
    {
        checkIndex_(index);
        return at_(index);
    }

    jxx::Ptr<CharBuffer> CharBuffer::get(const jxx::lang::CharArray dst)
    {
        if (!dst)
            throwIAE_("null array");
        return get(dst, 0, dst->length);
    }

    jxx::Ptr<CharBuffer> CharBuffer::get(const jxx::lang::CharArray dst,
                               jxx::lang::jint offset,
                               jxx::lang::jint length)
    {
        if (!dst)
            throwIAE_("null array");
        if (offset < 0 || length < 0 || offset > dst->length - length)
            throwIAE_("index out of bounds");
        checkRemaining_(length);
        for (jxx::lang::jint i = 0; i < length; ++i)
            (*dst)[offset + i] = at_(pos_ + i);
        pos_ += length;
        return self__();
    }

    jxx::Ptr<CharBuffer> CharBuffer::put(jxx::lang::jchar value)
    {
        checkWritable_();
        checkRemaining_(1);
        at_(pos_++) = value;
        return self__();
    }

    jxx::Ptr<CharBuffer> CharBuffer::put(jxx::lang::jint index, jxx::lang::jchar value)
    {
        checkWritable_();
        checkIndex_(index);
        at_(index) = value;
        return self__();
    }

    jxx::Ptr<CharBuffer> CharBuffer::put(const jxx::lang::CharArray src)
    {
        if (!src)
            throwIAE_("null array");
        return put(src, 0, src->length);
    }

    jxx::Ptr<CharBuffer> CharBuffer::put(const jxx::lang::CharArray src,
                               jxx::lang::jint offset,
                               jxx::lang::jint length)
    {
        if (!src)
            throwIAE_("null array");
        if (offset < 0 || length < 0 || offset > src->length - length)
            throwIAE_("index out of bounds");
        checkWritable_();
        checkRemaining_(length);
        for (jxx::lang::jint i = 0; i < length; ++i)
            at_(pos_ + i) = (*src)[offset + i];
        pos_ += length;
        return self__();
    }

    jxx::Ptr<CharBuffer> CharBuffer::put(jxx::Ptr<CharBuffer> src)
    {
        if (!src)
            throwIAE_("null source buffer");
        if (src.get() == this)
            throwIAE_("source buffer is this buffer");
        const auto n = src->remaining();
        checkWritable_();
        checkRemaining_(n);
        for (jxx::lang::jint i = 0; i < n; ++i)
            at_(pos_ + i) = src->get();
        pos_ += n;
        return self__();
    }

    jxx::Ptr<CharBuffer> CharBuffer::slice() const
    {
        auto out = std::shared_ptr<CharBuffer>(new CharBuffer(storage_, offset_ + pos_, remaining(), readOnly_));
        out->position(0);
        out->limit(remaining());
        return out;
    }

    jxx::Ptr<CharBuffer> CharBuffer::duplicate() const
    {
        auto out = std::shared_ptr<CharBuffer>(new CharBuffer(storage_, offset_, cap_, readOnly_));
        out->mark_ = mark_;
        out->pos_ = pos_;
        out->lim_ = lim_;
        return out;
    }

    jxx::Ptr<CharBuffer> CharBuffer::asReadOnlyBuffer() const
    {
        auto out = std::shared_ptr<CharBuffer>(new CharBuffer(storage_, offset_, cap_, true));
        out->mark_ = mark_;
        out->pos_ = pos_;
        out->lim_ = lim_;
        return out;
    }

    jxx::Ptr<CharBuffer> CharBuffer::compact()
    {
        checkWritable_();
        const auto rem = remaining();
        for (jxx::lang::jint i = 0; i < rem; ++i)
            at_(i) = at_(pos_ + i);
        pos_ = rem;
        lim_ = cap_;
        mark_ = -1;
        return self__();
    }

    jxx::lang::jbool CharBuffer::hasArray() const { return true; }

    jxx::lang::CharArray CharBuffer::array() const
    {
        auto out = jxx::NEW<jxx::lang::CharArrayType>(cap_);
        for (jxx::lang::jint i = 0; i < cap_; ++i)
            (*out)[i] = at_(i);
        return out;
    }

    jxx::lang::jint CharBuffer::arrayOffset() const { return 0; }
    jxx::lang::jbool CharBuffer::isReadOnly() const { return readOnly_; }
    jxx::lang::jbool CharBuffer::isDirect() const { return false; }
    jxx::Ptr<jxx::lang::String> CharBuffer::toString() const
    {
        auto arr = jxx::NEW<jxx::lang::CharArrayType>(remaining());
        for (jxx::lang::jint i = 0; i < remaining(); ++i)
            (*arr)[i] = at_(pos_ + i);
        return jxx::NEW<jxx::lang::String>(arr);
    }
}
