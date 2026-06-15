#include "nio/jxx.nio.IntBuffer.h"

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
    IntBuffer::IntBuffer(std::shared_ptr<std::vector<jxx::lang::jint>> storage,
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

    jxx::Ptr<IntBuffer> IntBuffer::allocate(jxx::lang::jint capacity)
    {
        if (capacity < 0)
            throwIAE_("capacity < 0");
        auto storage = std::make_shared<std::vector<jxx::lang::jint>>(static_cast<std::size_t>(capacity));
        return std::shared_ptr<IntBuffer>(new IntBuffer(storage, 0, capacity, false));
    }

    jxx::Ptr<IntBuffer> IntBuffer::wrap(const jxx::lang::IntArray array)
    {
        if (!array)
            throwIAE_("null array");
        return wrap(array, 0, array->length);
    }

    jxx::Ptr<IntBuffer> IntBuffer::wrap(const jxx::lang::IntArray array,
                                jxx::lang::jint offset,
                                jxx::lang::jint length)
    {
        if (!array)
            throwIAE_("null array");
        if (offset < 0 || length < 0 || offset > array->length - length)
            throwIAE_("index out of bounds");

        auto storage = std::make_shared<std::vector<jxx::lang::jint>>(static_cast<std::size_t>(array->length));
        for (jxx::lang::jint i = 0; i < array->length; ++i)
            (*storage)[static_cast<std::size_t>(i)] = (*array)[i];

        auto out = std::shared_ptr<IntBuffer>(new IntBuffer(storage, offset, length, false));
        out->position(0);
        out->limit(length);
        return out;
    }

    jxx::Ptr<IntBuffer> IntBuffer::self__()
    {
        if (thisPtr)
            return std::static_pointer_cast<IntBuffer>(thisPtr);
        return std::static_pointer_cast<IntBuffer>(shared_from_this());
    }

    void IntBuffer::checkWritable_() const
    {
        if (readOnly_)
            throwState_("buffer is read-only");
    }

    jxx::lang::jint& IntBuffer::at_(jxx::lang::jint index)
    {
        return (*storage_)[static_cast<std::size_t>(offset_ + index)];
    }

    const jxx::lang::jint& IntBuffer::at_(jxx::lang::jint index) const
    {
        return (*storage_)[static_cast<std::size_t>(offset_ + index)];
    }

    jxx::lang::jint IntBuffer::get()
    {
        checkRemaining_(1);
        return at_(pos_++);
    }

    jxx::lang::jint IntBuffer::get(jxx::lang::jint index) const
    {
        checkIndex_(index);
        return at_(index);
    }

    jxx::Ptr<IntBuffer> IntBuffer::get(const jxx::lang::IntArray dst)
    {
        if (!dst)
            throwIAE_("null array");
        return get(dst, 0, dst->length);
    }

    jxx::Ptr<IntBuffer> IntBuffer::get(const jxx::lang::IntArray dst,
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

    jxx::Ptr<IntBuffer> IntBuffer::put(jxx::lang::jint value)
    {
        checkWritable_();
        checkRemaining_(1);
        at_(pos_++) = value;
        return self__();
    }

    jxx::Ptr<IntBuffer> IntBuffer::put(jxx::lang::jint index, jxx::lang::jint value)
    {
        checkWritable_();
        checkIndex_(index);
        at_(index) = value;
        return self__();
    }

    jxx::Ptr<IntBuffer> IntBuffer::put(const jxx::lang::IntArray src)
    {
        if (!src)
            throwIAE_("null array");
        return put(src, 0, src->length);
    }

    jxx::Ptr<IntBuffer> IntBuffer::put(const jxx::lang::IntArray src,
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

    jxx::Ptr<IntBuffer> IntBuffer::put(jxx::Ptr<IntBuffer> src)
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

    jxx::Ptr<IntBuffer> IntBuffer::slice() const
    {
        auto out = std::shared_ptr<IntBuffer>(new IntBuffer(storage_, offset_ + pos_, remaining(), readOnly_));
        out->position(0);
        out->limit(remaining());
        return out;
    }

    jxx::Ptr<IntBuffer> IntBuffer::duplicate() const
    {
        auto out = std::shared_ptr<IntBuffer>(new IntBuffer(storage_, offset_, cap_, readOnly_));
        out->mark_ = mark_;
        out->pos_ = pos_;
        out->lim_ = lim_;
        return out;
    }

    jxx::Ptr<IntBuffer> IntBuffer::asReadOnlyBuffer() const
    {
        auto out = std::shared_ptr<IntBuffer>(new IntBuffer(storage_, offset_, cap_, true));
        out->mark_ = mark_;
        out->pos_ = pos_;
        out->lim_ = lim_;
        return out;
    }

    jxx::Ptr<IntBuffer> IntBuffer::compact()
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

    jxx::lang::jbool IntBuffer::hasArray() const { return true; }

    jxx::lang::IntArray IntBuffer::array() const
    {
        auto out = jxx::NEW<jxx::lang::IntArrayType>(cap_);
        for (jxx::lang::jint i = 0; i < cap_; ++i)
            (*out)[i] = at_(i);
        return out;
    }

    jxx::lang::jint IntBuffer::arrayOffset() const { return 0; }
    jxx::lang::jbool IntBuffer::isReadOnly() const { return readOnly_; }
    jxx::lang::jbool IntBuffer::isDirect() const { return false; }
}
