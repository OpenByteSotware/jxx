#pragma once

#include <memory>
#include <vector>

#include "jxx_types.h"
#include "jxx.lang.buildin_array.h"
#include "nio/jxx.nio.Buffer.h"
#include "jxx.lang.String.h"

namespace jxx::nio
{
    class CharBuffer final : public Buffer
    {
    public:
        ~CharBuffer() override = default;

        static jxx::Ptr<CharBuffer> allocate(jxx::lang::jint capacity);
        static jxx::Ptr<CharBuffer> wrap(const jxx::lang::CharArray array);
        static jxx::Ptr<CharBuffer> wrap(const jxx::lang::CharArray array,
                                    jxx::lang::jint offset,
                                    jxx::lang::jint length);
        static jxx::Ptr<CharBuffer> wrap(jxx::Ptr<jxx::lang::String> str);

        jxx::lang::jchar get();
        jxx::lang::jchar get(jxx::lang::jint index) const;
        jxx::Ptr<CharBuffer> get(const jxx::lang::CharArray dst);
        jxx::Ptr<CharBuffer> get(const jxx::lang::CharArray dst,
                            jxx::lang::jint offset,
                            jxx::lang::jint length);

        jxx::Ptr<CharBuffer> put(jxx::lang::jchar value);
        jxx::Ptr<CharBuffer> put(jxx::lang::jint index, jxx::lang::jchar value);
        jxx::Ptr<CharBuffer> put(const jxx::lang::CharArray src);
        jxx::Ptr<CharBuffer> put(const jxx::lang::CharArray src,
                            jxx::lang::jint offset,
                            jxx::lang::jint length);
        jxx::Ptr<CharBuffer> put(jxx::Ptr<CharBuffer> src);

        jxx::Ptr<CharBuffer> slice() const;
        jxx::Ptr<CharBuffer> duplicate() const;
        jxx::Ptr<CharBuffer> asReadOnlyBuffer() const;
        jxx::Ptr<CharBuffer> compact();

        jxx::lang::jbool hasArray() const override;
        jxx::lang::CharArray array() const;
        jxx::lang::jint arrayOffset() const;
        jxx::lang::jbool isReadOnly() const override;
        jxx::lang::jbool isDirect() const override;
        jxx::Ptr<jxx::lang::String> toString() const override;

    private:
        CharBuffer(std::shared_ptr<std::vector<jxx::lang::jchar>> storage,
              jxx::lang::jint offset,
              jxx::lang::jint capacity,
              jxx::lang::jbool readOnly);

        jxx::Ptr<CharBuffer> self__();
        void checkWritable_() const;
        jxx::lang::jchar& at_(jxx::lang::jint index);
        const jxx::lang::jchar& at_(jxx::lang::jint index) const;

    private:
        std::shared_ptr<std::vector<jxx::lang::jchar>> storage_;
        jxx::lang::jint offset_ = 0;
        jxx::lang::jbool readOnly_ = false;
    };
}
