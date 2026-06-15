#include "nio/jxx.nio.FloatBuffer.h"

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
    FloatBuffer::FloatBuffer(std::shared_ptr<std::vector<jxx::lang::jfloat>> storage,
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

    jxx::Ptr<FloatBuffer> FloatBuffer::allocate(jxx::lang::jint capacity)
    {
        if (capacity < 0)
            throwIAE_("capacity < 0");
        auto storage = std::make_shared<std::vector<jxx::lang::jfloat>>(static_cast<std::size_t>(capacity));
        return std::shared_ptr<FloatBuffer>(new FloatBuffer(storage, 0, capacity, false));
    }

    jxx::Ptr<FloatBuffer> FloatBuffer::wrap(const jxx::lang::FloatArray array)
    {
        if (!array)
            throwIAE_("null array");
        return wrap(array, 0, array->length);
    }

    jxx::Ptr<FloatBuffer> FloatBuffer::wrap(const jxx::lang::FloatArray array,
                                jxx::lang::jint offset,
                                jxx::lang::jint length)
    {
        if (!array)
            throwIAE_("null array");
        if (offset < 0 || length < 0 || offset > array->length - length)
            throwIAE_("index out of bounds");

        auto storage = std::make_shared<std::vector<jxx::lang::jfloat>>(static_cast<std::size_t>(array->length));
        for (jxx::lang::jint i = 0; i < array->length; ++i)
            (*storage)[static_cast<std::size_t>(i)] = (*array)[i];

        auto out = std::shared_ptr<FloatBuffer>(new FloatBuffer(storage, offset, length, false));
        out->position(0);
        out->limit(length);
        return out;
    }

    jxx::Ptr<FloatBuffer> FloatBuffer::self__()
    {
        if (thisPtr)
            return std::static_pointer_cast<FloatBuffer>(thisPtr);
        return std::static_pointer_cast<FloatBuffer>(shared_from_this());
    }

    void FloatBuffer::checkWritable_() const
    {
        if (readOnly_)
            throwState_("buffer is read-only");
    }

    jxx::lang::jfloat& FloatBuffer::at_(jxx::lang::jint index)
    {
        return (*storage_)[static_cast<std::size_t>(offset_ + index)];
    }

    const jxx::lang::jfloat& FloatBuffer::at_(jxx::lang::jint index) const
    {
        return (*storage_)[static_cast<std::size_t>(offset_ + index)];
    }

    jxx::lang::jfloat FloatBuffer::get()
    {
        checkRemaining_(1);
        return at_(pos_++);
    }

    jxx::lang::jfloat FloatBuffer::get(jxx::lang::jint index) const
    {
        checkIndex_(index);
        return at_(index);
    }

    jxx::Ptr<FloatBuffer> FloatBuffer::get(const jxx::lang::FloatArray dst)
    {
        if (!dst)
            throwIAE_("null array");
        return get(dst, 0, dst->length);
    }

    jxx::Ptr<FloatBuffer> FloatBuffer::get(const jxx::lang::FloatArray dst,
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

    jxx::Ptr<FloatBuffer> FloatBuffer::put(jxx::lang::jfloat value)
    {
        checkWritable_();
        checkRemaining_(1);
        at_(pos_++) = value;
        return self__();
    }

    jxx::Ptr<FloatBuffer> FloatBuffer::put(jxx::lang::jint index, jxx::lang::jfloat value)
    {
        checkWritable_();
        checkIndex_(index);
        at_(index) = value;
        return self__();
    }

    jxx::Ptr<FloatBuffer> FloatBuffer::put(const jxx::lang::FloatArray src)
    {
        if (!src)
            throwIAE_("null array");
        return put(src, 0, src->length);
    }

    jxx::Ptr<FloatBuffer> FloatBuffer::put(const jxx::lang::FloatArray src,
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

    jxx::Ptr<FloatBuffer> FloatBuffer::put(jxx::Ptr<FloatBuffer> src)
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

    jxx::Ptr<FloatBuffer> FloatBuffer::slice() const
    {
        auto out = std::shared_ptr<FloatBuffer>(new FloatBuffer(storage_, offset_ + pos_, remaining(), readOnly_));
        out->position(0);
        out->limit(remaining());
        return out;
    }

    jxx::Ptr<FloatBuffer> FloatBuffer::duplicate() const
    {
        auto out = std::shared_ptr<FloatBuffer>(new FloatBuffer(storage_, offset_, cap_, readOnly_));
        out->mark_ = mark_;
        out->pos_ = pos_;
        out->lim_ = lim_;
        return out;
    }

    jxx::Ptr<FloatBuffer> FloatBuffer::asReadOnlyBuffer() const
    {
        auto out = std::shared_ptr<FloatBuffer>(new FloatBuffer(storage_, offset_, cap_, true));
        out->mark_ = mark_;
        out->pos_ = pos_;
        out->lim_ = lim_;
        return out;
    }

    jxx::Ptr<FloatBuffer> FloatBuffer::compact()
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

    jxx::lang::jbool FloatBuffer::hasArray() const { return true; }

    jxx::lang::FloatArray FloatBuffer::array() const
    {
        auto out = jxx::NEW<jxx::lang::FloatArrayType>(cap_);
        for (jxx::lang::jint i = 0; i < cap_; ++i)
            (*out)[i] = at_(i);
        return out;
    }

    jxx::lang::jint FloatBuffer::arrayOffset() const { return 0; }
    jxx::lang::jbool FloatBuffer::isReadOnly() const { return readOnly_; }
    jxx::lang::jbool FloatBuffer::isDirect() const { return false; }
}
