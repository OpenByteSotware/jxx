#pragma once

#include <memory>
#include <vector>

#include "lang/jxx_types.h"
#include "lang/jxx.lang.buildin_array.h"
#include "nio/jxx.nio.Buffer.h"
#include "nio/jxx.nio.ByteOrder.h"

namespace jxx::nio
{
    class ByteBuffer : public Buffer
    {
    protected:
        ByteBuffer(std::shared_ptr<std::vector<jxx::lang::jbyte>> storage,
                   jxx::lang::jint offset,
                   jxx::lang::jint capacity,
                   jxx::lang::jbool readOnly,
                   jxx::lang::jbool direct,
                   jxx::Ptr<ByteOrder> order);

    public:
        ~ByteBuffer() override = default;

    public:
        static jxx::Ptr<ByteBuffer> allocate(jxx::lang::jint capacity);
        static jxx::Ptr<ByteBuffer> allocateDirect(jxx::lang::jint capacity);
        static jxx::Ptr<ByteBuffer> wrap(const jxx::lang::ByteArray array);
        static jxx::Ptr<ByteBuffer> wrap(const jxx::lang::ByteArray array,
                                         jxx::lang::jint offset,
                                         jxx::lang::jint length);

        jxx::lang::jbyte get();
        jxx::lang::jbyte get(jxx::lang::jint index) const;
        jxx::Ptr<ByteBuffer> get(const jxx::lang::ByteArray dst);
        jxx::Ptr<ByteBuffer> get(const jxx::lang::ByteArray dst,
                                 jxx::lang::jint offset,
                                 jxx::lang::jint length);

        jxx::Ptr<ByteBuffer> put(jxx::lang::jbyte value);
        jxx::Ptr<ByteBuffer> put(jxx::lang::jint index, jxx::lang::jbyte value);
        jxx::Ptr<ByteBuffer> put(const jxx::lang::ByteArray src);
        jxx::Ptr<ByteBuffer> put(const jxx::lang::ByteArray src,
                                 jxx::lang::jint offset,
                                 jxx::lang::jint length);
        jxx::Ptr<ByteBuffer> put(jxx::Ptr<ByteBuffer> src);

        jxx::Ptr<ByteBuffer> slice() const;
        jxx::Ptr<ByteBuffer> duplicate() const;
        jxx::Ptr<ByteBuffer> asReadOnlyBuffer() const;
        jxx::Ptr<ByteBuffer> compact();

        jxx::lang::jbool hasArray() const override;
        jxx::lang::ByteArray array() const;
        jxx::lang::jint arrayOffset() const;
        jxx::lang::jbool isReadOnly() const override;
        jxx::lang::jbool isDirect() const override;

        jxx::Ptr<ByteOrder> order() const;
        jxx::Ptr<ByteBuffer> order(jxx::Ptr<ByteOrder> order);

    protected:
        jxx::Ptr<ByteBuffer> self_Byte_();
        void checkWritable_() const;
        jxx::lang::jbyte& at_(jxx::lang::jint index);
        const jxx::lang::jbyte& at_(jxx::lang::jint index) const;

    protected:
        std::shared_ptr<std::vector<jxx::lang::jbyte>> storage_;
        jxx::lang::jint offset_ = 0;
        jxx::lang::jbool readOnly_ = false;
        jxx::lang::jbool direct_ = false;
        jxx::Ptr<ByteOrder> order_;
    };
}
