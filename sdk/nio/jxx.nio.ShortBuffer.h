#pragma once

#include <memory>
#include <vector>

#include "lang/jxx_types.h"
#include "lang/jxx.lang.buildin_array.h"
#include "nio/jxx.nio.Buffer.h"

namespace jxx::nio
{
    class ShortBuffer final : public Buffer
    {
    public:
        ~ShortBuffer() override = default;

        static jxx::Ptr<ShortBuffer> allocate(jxx::lang::jint capacity);
        static jxx::Ptr<ShortBuffer> wrap(const jxx::lang::ShortArray array);
        static jxx::Ptr<ShortBuffer> wrap(const jxx::lang::ShortArray array,
                                    jxx::lang::jint offset,
                                    jxx::lang::jint length);

        jxx::lang::jshort get();
        jxx::lang::jshort get(jxx::lang::jint index) const;
        jxx::Ptr<ShortBuffer> get(const jxx::lang::ShortArray dst);
        jxx::Ptr<ShortBuffer> get(const jxx::lang::ShortArray dst,
                            jxx::lang::jint offset,
                            jxx::lang::jint length);

        jxx::Ptr<ShortBuffer> put(jxx::lang::jshort value);
        jxx::Ptr<ShortBuffer> put(jxx::lang::jint index, jxx::lang::jshort value);
        jxx::Ptr<ShortBuffer> put(const jxx::lang::ShortArray src);
        jxx::Ptr<ShortBuffer> put(const jxx::lang::ShortArray src,
                            jxx::lang::jint offset,
                            jxx::lang::jint length);
        jxx::Ptr<ShortBuffer> put(jxx::Ptr<ShortBuffer> src);

        jxx::Ptr<ShortBuffer> slice() const;
        jxx::Ptr<ShortBuffer> duplicate() const;
        jxx::Ptr<ShortBuffer> asReadOnlyBuffer() const;
        jxx::Ptr<ShortBuffer> compact();

        jxx::lang::jbool hasArray() const override;
        jxx::lang::ShortArray array() const;
        jxx::lang::jint arrayOffset() const;
        jxx::lang::jbool isReadOnly() const override;
        jxx::lang::jbool isDirect() const override;

    private:
        ShortBuffer(std::shared_ptr<std::vector<jxx::lang::jshort>> storage,
              jxx::lang::jint offset,
              jxx::lang::jint capacity,
              jxx::lang::jbool readOnly);

        jxx::Ptr<ShortBuffer> self__();
        void checkWritable_() const;
        jxx::lang::jshort& at_(jxx::lang::jint index);
        const jxx::lang::jshort& at_(jxx::lang::jint index) const;

    private:
        std::shared_ptr<std::vector<jxx::lang::jshort>> storage_;
        jxx::lang::jint offset_ = 0;
        jxx::lang::jbool readOnly_ = false;
    };
}
