#pragma once

#include "lang/jxx_types.h"
#include "lang/jxx.lang.Object.h"

namespace jxx::nio
{
    class Buffer : public jxx::lang::Object
    {
    protected:
        Buffer(jxx::lang::jint mark,
               jxx::lang::jint pos,
               jxx::lang::jint lim,
               jxx::lang::jint cap);

    public:
        ~Buffer() override = default;

    public:
        jxx::lang::jint capacity() const noexcept;
        jxx::lang::jint position() const noexcept;
        jxx::Ptr<Buffer> position(jxx::lang::jint newPosition);

        jxx::lang::jint limit() const noexcept;
        jxx::Ptr<Buffer> limit(jxx::lang::jint newLimit);

        jxx::Ptr<Buffer> mark();
        jxx::Ptr<Buffer> reset();
        jxx::Ptr<Buffer> clear();
        jxx::Ptr<Buffer> flip();
        jxx::Ptr<Buffer> rewind();

        jxx::lang::jint remaining() const noexcept;
        jxx::lang::jbool hasRemaining() const noexcept;

        virtual jxx::lang::jbool isReadOnly() const = 0;
        virtual jxx::lang::jbool hasArray() const = 0;
        virtual jxx::lang::jbool isDirect() const = 0;

    protected:
        jxx::Ptr<Buffer> self_();
        void checkIndex_(jxx::lang::jint index) const;
        void checkIndex_(jxx::lang::jint index, jxx::lang::jint nb) const;
        void checkRemaining_(jxx::lang::jint nb) const;

    protected:
        jxx::lang::jint mark_;
        jxx::lang::jint pos_;
        jxx::lang::jint lim_;
        jxx::lang::jint cap_;
    };
}
