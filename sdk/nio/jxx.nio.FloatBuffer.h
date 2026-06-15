#pragma once

#include <memory>
#include <vector>

#include "jxx_types.h"
#include "jxx.lang.buildin_array.h"
#include "nio/jxx.nio.Buffer.h"

namespace jxx::nio
{
    class FloatBuffer final : public Buffer
    {
    public:
        ~FloatBuffer() override = default;

        static jxx::Ptr<FloatBuffer> allocate(jxx::lang::jint capacity);
        static jxx::Ptr<FloatBuffer> wrap(const jxx::lang::FloatArray array);
        static jxx::Ptr<FloatBuffer> wrap(const jxx::lang::FloatArray array,
                                    jxx::lang::jint offset,
                                    jxx::lang::jint length);

        jxx::lang::jfloat get();
        jxx::lang::jfloat get(jxx::lang::jint index) const;
        jxx::Ptr<FloatBuffer> get(const jxx::lang::FloatArray dst);
        jxx::Ptr<FloatBuffer> get(const jxx::lang::FloatArray dst,
                            jxx::lang::jint offset,
                            jxx::lang::jint length);

        jxx::Ptr<FloatBuffer> put(jxx::lang::jfloat value);
        jxx::Ptr<FloatBuffer> put(jxx::lang::jint index, jxx::lang::jfloat value);
        jxx::Ptr<FloatBuffer> put(const jxx::lang::FloatArray src);
        jxx::Ptr<FloatBuffer> put(const jxx::lang::FloatArray src,
                            jxx::lang::jint offset,
                            jxx::lang::jint length);
        jxx::Ptr<FloatBuffer> put(jxx::Ptr<FloatBuffer> src);

        jxx::Ptr<FloatBuffer> slice() const;
        jxx::Ptr<FloatBuffer> duplicate() const;
        jxx::Ptr<FloatBuffer> asReadOnlyBuffer() const;
        jxx::Ptr<FloatBuffer> compact();

        jxx::lang::jbool hasArray() const override;
        jxx::lang::FloatArray array() const;
        jxx::lang::jint arrayOffset() const;
        jxx::lang::jbool isReadOnly() const override;
        jxx::lang::jbool isDirect() const override;

    private:
        FloatBuffer(std::shared_ptr<std::vector<jxx::lang::jfloat>> storage,
              jxx::lang::jint offset,
              jxx::lang::jint capacity,
              jxx::lang::jbool readOnly);

        jxx::Ptr<FloatBuffer> self__();
        void checkWritable_() const;
        jxx::lang::jfloat& at_(jxx::lang::jint index);
        const jxx::lang::jfloat& at_(jxx::lang::jint index) const;

    private:
        std::shared_ptr<std::vector<jxx::lang::jfloat>> storage_;
        jxx::lang::jint offset_ = 0;
        jxx::lang::jbool readOnly_ = false;
    };
}
