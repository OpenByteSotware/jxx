#pragma once

#include <memory>
#include <vector>

#include "lang/jxx_types.h"
#include "lang/jxx.lang.buildin_array.h"
#include "nio/jxx.nio.Buffer.h"

namespace jxx::nio
{
    class LongBuffer final : public Buffer
    {
    public:
        ~LongBuffer() override = default;

        static jxx::Ptr<LongBuffer> allocate(jxx::lang::jint capacity);
        static jxx::Ptr<LongBuffer> wrap(const jxx::lang::LongArray array);
        static jxx::Ptr<LongBuffer> wrap(const jxx::lang::LongArray array,
                                    jxx::lang::jint offset,
                                    jxx::lang::jint length);

        jxx::lang::jlong get();
        jxx::lang::jlong get(jxx::lang::jint index) const;
        jxx::Ptr<LongBuffer> get(const jxx::lang::LongArray dst);
        jxx::Ptr<LongBuffer> get(const jxx::lang::LongArray dst,
                            jxx::lang::jint offset,
                            jxx::lang::jint length);

        jxx::Ptr<LongBuffer> put(jxx::lang::jlong value);
        jxx::Ptr<LongBuffer> put(jxx::lang::jint index, jxx::lang::jlong value);
        jxx::Ptr<LongBuffer> put(const jxx::lang::LongArray src);
        jxx::Ptr<LongBuffer> put(const jxx::lang::LongArray src,
                            jxx::lang::jint offset,
                            jxx::lang::jint length);
        jxx::Ptr<LongBuffer> put(jxx::Ptr<LongBuffer> src);

        jxx::Ptr<LongBuffer> slice() const;
        jxx::Ptr<LongBuffer> duplicate() const;
        jxx::Ptr<LongBuffer> asReadOnlyBuffer() const;
        jxx::Ptr<LongBuffer> compact();

        jxx::lang::jbool hasArray() const override;
        jxx::lang::LongArray array() const;
        jxx::lang::jint arrayOffset() const;
        jxx::lang::jbool isReadOnly() const override;
        jxx::lang::jbool isDirect() const override;

    private:
        LongBuffer(std::shared_ptr<std::vector<jxx::lang::jlong>> storage,
              jxx::lang::jint offset,
              jxx::lang::jint capacity,
              jxx::lang::jbool readOnly);

        jxx::Ptr<LongBuffer> self__();
        void checkWritable_() const;
        jxx::lang::jlong& at_(jxx::lang::jint index);
        const jxx::lang::jlong& at_(jxx::lang::jint index) const;

    private:
        std::shared_ptr<std::vector<jxx::lang::jlong>> storage_;
        jxx::lang::jint offset_ = 0;
        jxx::lang::jbool readOnly_ = false;
    };
}
