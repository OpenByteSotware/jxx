#pragma once

#include "lang/jxx_types.h"
#include "lang/jxx.lang.Object.h"
#include "lang/jxx.lang.String.h"

namespace jxx::nio::charset
{
    class CoderResult final : public jxx::lang::Object
    {
    public:
        static jxx::Ptr<CoderResult> JXX_UNDERFLOW;
        static jxx::Ptr<CoderResult> JXX_OVERFLOW;

    public:
        explicit CoderResult(jxx::Ptr<jxx::lang::String> kind,
                             jxx::lang::jint length = 0);
        ~CoderResult() override = default;

    public:
        static jxx::Ptr<CoderResult> malformedForLength(jxx::lang::jint length);
        static jxx::Ptr<CoderResult> unmappableForLength(jxx::lang::jint length);

        jxx::lang::jbool isUnderflow() const;
        jxx::lang::jbool isOverflow() const;
        jxx::lang::jbool isError() const;
        jxx::lang::jbool isMalformed() const;
        jxx::lang::jbool isUnmappable() const;
        jxx::lang::jint length() const;

        jxx::Ptr<jxx::lang::String> toString() const override;

    private:
        jxx::Ptr<jxx::lang::String> kind_;
        jxx::lang::jint length_ = 0;
    };
}
