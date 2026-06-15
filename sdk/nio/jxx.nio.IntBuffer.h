#pragma once

#include <memory>
#include <vector>

#include "lang/jxx_types.h"
#include "lang/jxx.lang.buildin_array.h"
#include "nio/jxx.nio.Buffer.h"

namespace jxx::nio
{
    class IntBuffer final : public Buffer
    {
    public:
        ~IntBuffer() override = default;

        static jxx::Ptr<IntBuffer> allocate(jxx::lang::jint capacity);
        static jxx::Ptr<IntBuffer> wrap(const jxx::lang::IntArray array);
        static jxx::Ptr<IntBuffer> wrap(const jxx::lang::IntArray array,
                                    jxx::lang::jint offset,
                                    jxx::lang::jint length);

        jxx::lang::jint get();
        jxx::lang::jint get(jxx::lang::jint index) const;
        jxx::Ptr<IntBuffer> get(const jxx::lang::IntArray dst);
        jxx::Ptr<IntBuffer> get(const jxx::lang::IntArray dst,
                            jxx::lang::jint offset,
                            jxx::lang::jint length);

        jxx::Ptr<IntBuffer> put(jxx::lang::jint value);
        jxx::Ptr<IntBuffer> put(jxx::lang::jint index, jxx::lang::jint value);
        jxx::Ptr<IntBuffer> put(const jxx::lang::IntArray src);
        jxx::Ptr<IntBuffer> put(const jxx::lang::IntArray src,
                            jxx::lang::jint offset,
                            jxx::lang::jint length);
        jxx::Ptr<IntBuffer> put(jxx::Ptr<IntBuffer> src);

        jxx::Ptr<IntBuffer> slice() const;
        jxx::Ptr<IntBuffer> duplicate() const;
        jxx::Ptr<IntBuffer> asReadOnlyBuffer() const;
        jxx::Ptr<IntBuffer> compact();

        jxx::lang::jbool hasArray() const override;
        jxx::lang::IntArray array() const;
        jxx::lang::jint arrayOffset() const;
        jxx::lang::jbool isReadOnly() const override;
        jxx::lang::jbool isDirect() const override;

    private:
        IntBuffer(std::shared_ptr<std::vector<jxx::lang::jint>> storage,
              jxx::lang::jint offset,
              jxx::lang::jint capacity,
              jxx::lang::jbool readOnly);

        jxx::Ptr<IntBuffer> self__();
        void checkWritable_() const;
        jxx::lang::jint& at_(jxx::lang::jint index);
        const jxx::lang::jint& at_(jxx::lang::jint index) const;

    private:
        std::shared_ptr<std::vector<jxx::lang::jint>> storage_;
        jxx::lang::jint offset_ = 0;
        jxx::lang::jbool readOnly_ = false;
    };
}
