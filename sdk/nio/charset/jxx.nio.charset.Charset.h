#pragma once

#include <vector>

#include "lang/jxx_types.h"
#include "lang/jxx.lang.buildin_array.h"
#include "lang/jxx.lang.Object.h"

namespace jxx::nio
{
    class ByteBuffer;
    class CharBuffer;
}

namespace jxx::lang {
    class ClassAny;
    class String;
}

namespace jxx::nio::charset
{
    class CharsetDecoder;
    class CharsetEncoder;

    class Charset : public jxx::lang::Object
    {
    protected:
        Charset(jxx::Ptr<jxx::lang::String> canonicalName,
                const jxx::Ptr<jxx::lang::JxxArray<jxx::Ptr<jxx::lang::String>, 1U>> aliases);

    public:
        ~Charset() override = default;

    public:
        static jxx::lang::jbool isSupported(jxx::Ptr<jxx::lang::String> charsetName);
        static jxx::Ptr<Charset> forName(jxx::Ptr<jxx::lang::String> charsetName);
        static jxx::Ptr<Charset> defaultCharset();

        jxx::Ptr<jxx::lang::String> name() const;
        virtual jxx::lang::jbool contains(jxx::Ptr<Charset> cs) const;
        virtual jxx::Ptr<CharsetDecoder> newDecoder();
        virtual jxx::Ptr<CharsetEncoder> newEncoder();

        jxx::Ptr<jxx::lang::String> toString() const override;
        jxx::lang::jbool equals(jxx::Ptr<jxx::lang::Object> other) const override;
        jxx::lang::jint hashCode() const override;

    protected:
        jxx::Ptr<jxx::lang::String> canonicalName_;
        jxx::Ptr<jxx::lang::JxxArray<jxx::Ptr<jxx::lang::String>, 1U>> aliases_;
    };
}
