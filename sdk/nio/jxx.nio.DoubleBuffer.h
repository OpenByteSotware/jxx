#pragma once

#include <memory>
#include <vector>

#include "lang/jxx_types.h"
#include "lang/jxx.lang.JxxArray.h"
#include "nio/jxx.nio.Buffer.h"

namespace jxx::nio
{
    class DoubleBuffer final : public jxx::lang::ClassBase<DoubleBuffer, Buffer>
    {
    public:
        using JxxSuper = Buffer;
        using Super = jxx::lang::ClassBase<DoubleBuffer, JxxSuper>;

        ~DoubleBuffer() override = default;

        static jxx::Ptr<DoubleBuffer> allocate(jxx::lang::jint capacity);
        static jxx::Ptr<DoubleBuffer> wrap(const jxx::lang::DoubleArray& array);
        static jxx::Ptr<DoubleBuffer> wrap(const jxx::lang::DoubleArray& array,
                                    jxx::lang::jint offset,
                                    jxx::lang::jint length);

        jxx::lang::jdouble get();
        jxx::lang::jdouble get(jxx::lang::jint index) const;
        jxx::Ptr<DoubleBuffer> get(const jxx::lang::DoubleArray& dst);
        jxx::Ptr<DoubleBuffer> get(const jxx::lang::DoubleArray& dst,
                            jxx::lang::jint offset,
                            jxx::lang::jint length);

        jxx::Ptr<DoubleBuffer> put(jxx::lang::jdouble value);
        jxx::Ptr<DoubleBuffer> put(jxx::lang::jint index, jxx::lang::jdouble value);
        jxx::Ptr<DoubleBuffer> put(const jxx::lang::DoubleArray& src);
        jxx::Ptr<DoubleBuffer> put(const jxx::lang::DoubleArray& src,
                            jxx::lang::jint offset,
                            jxx::lang::jint length);
        jxx::Ptr<DoubleBuffer> put(const jxx::Ptr<DoubleBuffer>& src);

        jxx::Ptr<DoubleBuffer> slice() const;
        jxx::Ptr<DoubleBuffer> duplicate() const;
        jxx::Ptr<DoubleBuffer> asReadOnlyBuffer() const;
        jxx::Ptr<DoubleBuffer> compact();

        jxx::lang::jbool hasArray() const override;
        jxx::lang::DoubleArray array() const;
        jxx::lang::jint arrayOffset() const;
        jxx::lang::jbool isReadOnly() const override;
        jxx::lang::jbool isDirect() const override;

    private:
        DoubleBuffer(const jxx::lang::DoubleArray& storage,
              jxx::lang::jint offset,
              jxx::lang::jint capacity,
              jxx::lang::jbool readOnly);

        jxx::Ptr<DoubleBuffer> self__();
        void checkWritable_() const;
        jxx::lang::jdouble& at_(jxx::lang::jint index);
        const jxx::lang::jdouble& at_(jxx::lang::jint index) const;

    private:
        jxx::lang::DoubleArray storage_;
        jxx::lang::jint offset_ = 0;
        jxx::lang::jbool readOnly_ = false;
    };
}
