#include "nio/jxx.nio.ByteBuffer.h"

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
    ByteBuffer::ByteBuffer(std::shared_ptr<std::vector<jxx::lang::jbyte>> storage,
                           jxx::lang::jint offset,
                           jxx::lang::jint capacity,
                           jxx::lang::jbool readOnly,
                           jxx::lang::jbool direct,
                           jxx::Ptr<ByteOrder> order)
        : Buffer(-1, 0, capacity, capacity),
          storage_(std::move(storage)),
          offset_(offset),
          readOnly_(readOnly),
          direct_(direct),
          order_(std::move(order))
    {
        if (!storage_)
            throwIAE_("null storage");
        if (!order_)
            order_ = ByteOrder::BIG_ENDIAN;
    }

    jxx::Ptr<ByteBuffer> ByteBuffer::allocate(jxx::lang::jint capacity)
    {
        if (capacity < 0)
            throwIAE_("capacity < 0");
        auto storage = std::make_shared<std::vector<jxx::lang::jbyte>>(static_cast<std::size_t>(capacity));
        return std::shared_ptr<ByteBuffer>(new ByteBuffer(storage, 0, capacity, false, false, ByteOrder::BIG_ENDIAN));
    }

    jxx::Ptr<ByteBuffer> ByteBuffer::allocateDirect(jxx::lang::jint capacity)
    {
        if (capacity < 0)
            throwIAE_("capacity < 0");
        auto storage = std::make_shared<std::vector<jxx::lang::jbyte>>(static_cast<std::size_t>(capacity));
        return std::shared_ptr<ByteBuffer>(new ByteBuffer(storage, 0, capacity, false, true, ByteOrder::BIG_ENDIAN));
    }

    jxx::Ptr<ByteBuffer> ByteBuffer::wrap(const jxx::lang::ByteArray array)
    {
        if (!array)
            throwIAE_("null array");
        return wrap(array, 0, array->length);
    }

    jxx::Ptr<ByteBuffer> ByteBuffer::wrap(const jxx::lang::ByteArray array,
                                          jxx::lang::jint offset,
                                          jxx::lang::jint length)
    {
        if (!array)
            throwIAE_("null array");
        if (offset < 0 || length < 0 || offset > array->length - length)
            throwIAE_("index out of bounds");

        auto storage = std::make_shared<std::vector<jxx::lang::jbyte>>(static_cast<std::size_t>(array->length));
        for (jxx::lang::jint i = 0; i < array->length; ++i)
            (*storage)[static_cast<std::size_t>(i)] = (*array)[i];

        auto out = std::shared_ptr<ByteBuffer>(new ByteBuffer(storage, offset, length, false, false, ByteOrder::BIG_ENDIAN));
        out->position(0);
        out->limit(length);
        return out;
    }

    jxx::Ptr<ByteBuffer> ByteBuffer::self_Byte_()
    {
        if (thisPtr)
            return std::static_pointer_cast<ByteBuffer>(thisPtr);
        return std::static_pointer_cast<ByteBuffer>(shared_from_this());
    }

    void ByteBuffer::checkWritable_() const
    {
        if (readOnly_)
            throwState_("buffer is read-only");
    }

    jxx::lang::jbyte& ByteBuffer::at_(jxx::lang::jint index)
    {
        return (*storage_)[static_cast<std::size_t>(offset_ + index)];
    }

    const jxx::lang::jbyte& ByteBuffer::at_(jxx::lang::jint index) const
    {
        return (*storage_)[static_cast<std::size_t>(offset_ + index)];
    }

    jxx::lang::jbyte ByteBuffer::get()
    {
        checkRemaining_(1);
        return at_(pos_++);
    }

    jxx::lang::jbyte ByteBuffer::get(jxx::lang::jint index) const
    {
        checkIndex_(index);
        return at_(index);
    }

    jxx::Ptr<ByteBuffer> ByteBuffer::get(const jxx::lang::ByteArray dst)
    {
        if (!dst)
            throwIAE_("null array");
        return get(dst, 0, dst->length);
    }

    jxx::Ptr<ByteBuffer> ByteBuffer::get(const jxx::lang::ByteArray dst,
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
        return self_Byte_();
    }

    jxx::Ptr<ByteBuffer> ByteBuffer::put(jxx::lang::jbyte value)
    {
        checkWritable_();
        checkRemaining_(1);
        at_(pos_++) = value;
        return self_Byte_();
    }

    jxx::Ptr<ByteBuffer> ByteBuffer::put(jxx::lang::jint index, jxx::lang::jbyte value)
    {
        checkWritable_();
        checkIndex_(index);
        at_(index) = value;
        return self_Byte_();
    }

    jxx::Ptr<ByteBuffer> ByteBuffer::put(const jxx::lang::ByteArray src)
    {
        if (!src)
            throwIAE_("null array");
        return put(src, 0, src->length);
    }

    jxx::Ptr<ByteBuffer> ByteBuffer::put(const jxx::lang::ByteArray src,
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
        return self_Byte_();
    }

    jxx::Ptr<ByteBuffer> ByteBuffer::put(jxx::Ptr<ByteBuffer> src)
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
        return self_Byte_();
    }

    jxx::Ptr<ByteBuffer> ByteBuffer::slice() const
    {
        auto out = std::shared_ptr<ByteBuffer>(new ByteBuffer(storage_, offset_ + pos_, remaining(), readOnly_, direct_, order_));
        out->position(0);
        out->limit(remaining());
        return out;
    }

    jxx::Ptr<ByteBuffer> ByteBuffer::duplicate() const
    {
        auto out = std::shared_ptr<ByteBuffer>(new ByteBuffer(storage_, offset_, cap_, readOnly_, direct_, order_));
        out->mark_ = mark_;
        out->pos_ = pos_;
        out->lim_ = lim_;
        return out;
    }

    jxx::Ptr<ByteBuffer> ByteBuffer::asReadOnlyBuffer() const
    {
        auto out = std::shared_ptr<ByteBuffer>(new ByteBuffer(storage_, offset_, cap_, true, direct_, order_));
        out->mark_ = mark_;
        out->pos_ = pos_;
        out->lim_ = lim_;
        return out;
    }

    jxx::Ptr<ByteBuffer> ByteBuffer::compact()
    {
        checkWritable_();
        const auto rem = remaining();
        for (jxx::lang::jint i = 0; i < rem; ++i)
            at_(i) = at_(pos_ + i);
        pos_ = rem;
        lim_ = cap_;
        mark_ = -1;
        return self_Byte_();
    }

    jxx::lang::jbool ByteBuffer::hasArray() const { return !direct_; }

    jxx::lang::ByteArray ByteBuffer::array() const
    {
        if (!hasArray())
            throwState_("buffer has no accessible backing array");
        auto out = jxx::NEW<jxx::lang::ByteArrayType>(cap_);
        for (jxx::lang::jint i = 0; i < cap_; ++i)
            (*out)[i] = at_(i);
        return out;
    }

    jxx::lang::jint ByteBuffer::arrayOffset() const
    {
        if (!hasArray())
            throwState_("buffer has no accessible backing array");
        return 0;
    }

    jxx::lang::jbool ByteBuffer::isReadOnly() const { return readOnly_; }
    jxx::lang::jbool ByteBuffer::isDirect() const { return direct_; }
    jxx::Ptr<ByteOrder> ByteBuffer::order() const { return order_; }

    jxx::Ptr<ByteBuffer> ByteBuffer::order(jxx::Ptr<ByteOrder> order)
    {
        order_ = order ? order : ByteOrder::BIG_ENDIAN;
        return self_Byte_();
    }
}
