#include "nio/jxx.nio.Buffer.h"

#include <stdexcept>

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

namespace jxx::nio
{
    Buffer::Buffer(jxx::lang::jint mark,
                   jxx::lang::jint pos,
                   jxx::lang::jint lim,
                   jxx::lang::jint cap)
        : mark_(mark), pos_(pos), lim_(lim), cap_(cap)
    {
        if (cap < 0)
            throwIAE_("capacity < 0");
        if (lim < 0 || lim > cap)
            throwIAE_("limit out of range");
        if (pos < 0 || pos > lim)
            throwIAE_("position out of range");
        if (mark >= 0 && mark > pos)
            throwIAE_("mark > position");
    }

    jxx::lang::jint Buffer::capacity() const noexcept { return cap_; }
    jxx::lang::jint Buffer::position() const noexcept { return pos_; }
    jxx::lang::jint Buffer::limit() const noexcept { return lim_; }
    jxx::lang::jint Buffer::remaining() const noexcept { return lim_ - pos_; }
    jxx::lang::jbool Buffer::hasRemaining() const noexcept { return remaining() > 0; }

    jxx::Ptr<Buffer> Buffer::self_()
    {
        return jxx::CAST<Buffer, jxx::lang::Object>(this->thisPtr);
    }

    jxx::Ptr<Buffer> Buffer::position(jxx::lang::jint newPosition)
    {
        if (newPosition < 0 || newPosition > lim_)
            throwIAE_("position out of range");
        pos_ = newPosition;
        if (mark_ > pos_)
            mark_ = -1;
        return self_();
    }

    jxx::Ptr<Buffer> Buffer::limit(jxx::lang::jint newLimit)
    {
        if (newLimit < 0 || newLimit > cap_)
            throwIAE_("limit out of range");
        lim_ = newLimit;
        if (pos_ > lim_)
            pos_ = lim_;
        if (mark_ > lim_)
            mark_ = -1;
        return self_();
    }

    jxx::Ptr<Buffer> Buffer::mark()
    {
        mark_ = pos_;
        return self_();
    }

    jxx::Ptr<Buffer> Buffer::reset()
    {
        if (mark_ < 0)
            throwIOE_("mark not set");
        pos_ = mark_;
        return self_();
    }

    jxx::Ptr<Buffer> Buffer::clear()
    {
        pos_ = 0;
        lim_ = cap_;
        mark_ = -1;
        return self_();
    }

    jxx::Ptr<Buffer> Buffer::flip()
    {
        lim_ = pos_;
        pos_ = 0;
        mark_ = -1;
        return self_();
    }

    jxx::Ptr<Buffer> Buffer::rewind()
    {
        pos_ = 0;
        mark_ = -1;
        return self_();
    }

    void Buffer::checkIndex_(jxx::lang::jint index) const
    {
        if (index < 0 || index >= lim_)
            throwIAE_("index out of bounds");
    }

    void Buffer::checkIndex_(jxx::lang::jint index, jxx::lang::jint nb) const
    {
        if (index < 0 || nb < 0 || index > lim_ - nb)
            throwIAE_("index out of bounds");
    }

    void Buffer::checkRemaining_(jxx::lang::jint nb) const
    {
        if (nb < 0 || remaining() < nb)
            throwIOE_("buffer underflow/overflow");
    }
}
