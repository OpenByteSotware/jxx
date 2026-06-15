#include "nio/jxx.nio.ShortBuffer.h"

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
    ShortBuffer::ShortBuffer(std::shared_ptr<std::vector<jxx::lang::jshort>> storage,
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

    jxx::Ptr<ShortBuffer> ShortBuffer::allocate(jxx::lang::jint capacity)
    {
        if (capacity < 0)
            throwIAE_("capacity < 0");
        auto storage = std::make_shared<std::vector<jxx::lang::jshort>>(static_cast<std::size_t>(capacity));
        return std::shared_ptr<ShortBuffer>(new ShortBuffer(storage, 0, capacity, false));
    }

    jxx::Ptr<ShortBuffer> ShortBuffer::wrap(const jxx::lang::ShortArray array)
    {
        if (!array)
            throwIAE_("null array");
        return wrap(array, 0, array->length);
    }

    jxx::Ptr<ShortBuffer> ShortBuffer::wrap(const jxx::lang::ShortArray array,
                                jxx::lang::jint offset,
                                jxx::lang::jint length)
    {
        if (!array)
            throwIAE_("null array");
        if (offset < 0 || length < 0 || offset > array->length - length)
            throwIAE_("index out of bounds");

        auto storage = std::make_shared<std::vector<jxx::lang::jshort>>(static_cast<std::size_t>(array->length));
        for (jxx::lang::jint i = 0; i < array->length; ++i)
            (*storage)[static_cast<std::size_t>(i)] = (*array)[i];

        auto out = std::shared_ptr<ShortBuffer>(new ShortBuffer(storage, offset, length, false));
        out->position(0);
        out->limit(length);
        return out;
    }

    jxx::Ptr<ShortBuffer> ShortBuffer::self__()
    {
        if (thisPtr)
            return std::static_pointer_cast<ShortBuffer>(thisPtr);
        return std::static_pointer_cast<ShortBuffer>(shared_from_this());
    }

    void ShortBuffer::checkWritable_() const
    {
        if (readOnly_)
            throwState_("buffer is read-only");
    }

    jxx::lang::jshort& ShortBuffer::at_(jxx::lang::jint index)
    {
        return (*storage_)[static_cast<std::size_t>(offset_ + index)];
    }

    const jxx::lang::jshort& ShortBuffer::at_(jxx::lang::jint index) const
    {
        return (*storage_)[static_cast<std::size_t>(offset_ + index)];
    }

    jxx::lang::jshort ShortBuffer::get()
    {
        checkRemaining_(1);
        return at_(pos_++);
    }

    jxx::lang::jshort ShortBuffer::get(jxx::lang::jint index) const
    {
        checkIndex_(index);
        return at_(index);
    }

    jxx::Ptr<ShortBuffer> ShortBuffer::get(const jxx::lang::ShortArray dst)
    {
        if (!dst)
            throwIAE_("null array");
        return get(dst, 0, dst->length);
    }

    jxx::Ptr<ShortBuffer> ShortBuffer::get(const jxx::lang::ShortArray dst,
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

    jxx::Ptr<ShortBuffer> ShortBuffer::put(jxx::lang::jshort value)
    {
        checkWritable_();
        checkRemaining_(1);
        at_(pos_++) = value;
        return self__();
    }

    jxx::Ptr<ShortBuffer> ShortBuffer::put(jxx::lang::jint index, jxx::lang::jshort value)
    {
        checkWritable_();
        checkIndex_(index);
        at_(index) = value;
        return self__();
    }

    jxx::Ptr<ShortBuffer> ShortBuffer::put(const jxx::lang::ShortArray src)
    {
        if (!src)
            throwIAE_("null array");
        return put(src, 0, src->length);
    }

    jxx::Ptr<ShortBuffer> ShortBuffer::put(const jxx::lang::ShortArray src,
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

    jxx::Ptr<ShortBuffer> ShortBuffer::put(jxx::Ptr<ShortBuffer> src)
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

    jxx::Ptr<ShortBuffer> ShortBuffer::slice() const
    {
        auto out = std::shared_ptr<ShortBuffer>(new ShortBuffer(storage_, offset_ + pos_, remaining(), readOnly_));
        out->position(0);
        out->limit(remaining());
        return out;
    }

    jxx::Ptr<ShortBuffer> ShortBuffer::duplicate() const
    {
        auto out = std::shared_ptr<ShortBuffer>(new ShortBuffer(storage_, offset_, cap_, readOnly_));
        out->mark_ = mark_;
        out->pos_ = pos_;
        out->lim_ = lim_;
        return out;
    }

    jxx::Ptr<ShortBuffer> ShortBuffer::asReadOnlyBuffer() const
    {
        auto out = std::shared_ptr<ShortBuffer>(new ShortBuffer(storage_, offset_, cap_, true));
        out->mark_ = mark_;
        out->pos_ = pos_;
        out->lim_ = lim_;
        return out;
    }

    jxx::Ptr<ShortBuffer> ShortBuffer::compact()
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

    jxx::lang::jbool ShortBuffer::hasArray() const { return true; }

    jxx::lang::ShortArray ShortBuffer::array() const
    {
        auto out = jxx::NEW<jxx::lang::ShortArrayType>(cap_);
        for (jxx::lang::jint i = 0; i < cap_; ++i)
            (*out)[i] = at_(i);
        return out;
    }

    jxx::lang::jint ShortBuffer::arrayOffset() const { return 0; }
    jxx::lang::jbool ShortBuffer::isReadOnly() const { return readOnly_; }
    jxx::lang::jbool ShortBuffer::isDirect() const { return false; }
}
