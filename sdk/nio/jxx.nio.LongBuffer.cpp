#include "nio/jxx.nio.LongBuffer.h"

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
    LongBuffer::LongBuffer(std::shared_ptr<std::vector<jxx::lang::jlong>> storage,
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

    jxx::Ptr<LongBuffer> LongBuffer::allocate(jxx::lang::jint capacity)
    {
        if (capacity < 0)
            throwIAE_("capacity < 0");
        auto storage = std::make_shared<std::vector<jxx::lang::jlong>>(static_cast<std::size_t>(capacity));
        return std::shared_ptr<LongBuffer>(new LongBuffer(storage, 0, capacity, false));
    }

    jxx::Ptr<LongBuffer> LongBuffer::wrap(const jxx::lang::LongArray array)
    {
        if (!array)
            throwIAE_("null array");
        return wrap(array, 0, array->length);
    }

    jxx::Ptr<LongBuffer> LongBuffer::wrap(const jxx::lang::LongArray array,
                                jxx::lang::jint offset,
                                jxx::lang::jint length)
    {
        if (!array)
            throwIAE_("null array");
        if (offset < 0 || length < 0 || offset > array->length - length)
            throwIAE_("index out of bounds");

        auto storage = std::make_shared<std::vector<jxx::lang::jlong>>(static_cast<std::size_t>(array->length));
        for (jxx::lang::jint i = 0; i < array->length; ++i)
            (*storage)[static_cast<std::size_t>(i)] = (*array)[i];

        auto out = std::shared_ptr<LongBuffer>(new LongBuffer(storage, offset, length, false));
        out->position(0);
        out->limit(length);
        return out;
    }

    jxx::Ptr<LongBuffer> LongBuffer::self__()
    {
        if (thisPtr)
            return std::static_pointer_cast<LongBuffer>(thisPtr);
        return std::static_pointer_cast<LongBuffer>(shared_from_this());
    }

    void LongBuffer::checkWritable_() const
    {
        if (readOnly_)
            throwState_("buffer is read-only");
    }

    jxx::lang::jlong& LongBuffer::at_(jxx::lang::jint index)
    {
        return (*storage_)[static_cast<std::size_t>(offset_ + index)];
    }

    const jxx::lang::jlong& LongBuffer::at_(jxx::lang::jint index) const
    {
        return (*storage_)[static_cast<std::size_t>(offset_ + index)];
    }

    jxx::lang::jlong LongBuffer::get()
    {
        checkRemaining_(1);
        return at_(pos_++);
    }

    jxx::lang::jlong LongBuffer::get(jxx::lang::jint index) const
    {
        checkIndex_(index);
        return at_(index);
    }

    jxx::Ptr<LongBuffer> LongBuffer::get(const jxx::lang::LongArray dst)
    {
        if (!dst)
            throwIAE_("null array");
        return get(dst, 0, dst->length);
    }

    jxx::Ptr<LongBuffer> LongBuffer::get(const jxx::lang::LongArray dst,
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

    jxx::Ptr<LongBuffer> LongBuffer::put(jxx::lang::jlong value)
    {
        checkWritable_();
        checkRemaining_(1);
        at_(pos_++) = value;
        return self__();
    }

    jxx::Ptr<LongBuffer> LongBuffer::put(jxx::lang::jint index, jxx::lang::jlong value)
    {
        checkWritable_();
        checkIndex_(index);
        at_(index) = value;
        return self__();
    }

    jxx::Ptr<LongBuffer> LongBuffer::put(const jxx::lang::LongArray src)
    {
        if (!src)
            throwIAE_("null array");
        return put(src, 0, src->length);
    }

    jxx::Ptr<LongBuffer> LongBuffer::put(const jxx::lang::LongArray src,
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

    jxx::Ptr<LongBuffer> LongBuffer::put(jxx::Ptr<LongBuffer> src)
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

    jxx::Ptr<LongBuffer> LongBuffer::slice() const
    {
        auto out = std::shared_ptr<LongBuffer>(new LongBuffer(storage_, offset_ + pos_, remaining(), readOnly_));
        out->position(0);
        out->limit(remaining());
        return out;
    }

    jxx::Ptr<LongBuffer> LongBuffer::duplicate() const
    {
        auto out = std::shared_ptr<LongBuffer>(new LongBuffer(storage_, offset_, cap_, readOnly_));
        out->mark_ = mark_;
        out->pos_ = pos_;
        out->lim_ = lim_;
        return out;
    }

    jxx::Ptr<LongBuffer> LongBuffer::asReadOnlyBuffer() const
    {
        auto out = std::shared_ptr<LongBuffer>(new LongBuffer(storage_, offset_, cap_, true));
        out->mark_ = mark_;
        out->pos_ = pos_;
        out->lim_ = lim_;
        return out;
    }

    jxx::Ptr<LongBuffer> LongBuffer::compact()
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

    jxx::lang::jbool LongBuffer::hasArray() const { return true; }

    jxx::lang::LongArray LongBuffer::array() const
    {
        auto out = jxx::NEW<jxx::lang::LongArrayType>(cap_);
        for (jxx::lang::jint i = 0; i < cap_; ++i)
            (*out)[i] = at_(i);
        return out;
    }

    jxx::lang::jint LongBuffer::arrayOffset() const { return 0; }
    jxx::lang::jbool LongBuffer::isReadOnly() const { return readOnly_; }
    jxx::lang::jbool LongBuffer::isDirect() const { return false; }
}
