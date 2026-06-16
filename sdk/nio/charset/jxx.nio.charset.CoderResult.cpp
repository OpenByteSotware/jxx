#include "nio/charset/jxx.nio.charset.CoderResult.h"

namespace jxx::nio::charset
{
    jxx::Ptr<CoderResult> CoderResult::JXX_UNDERFLOW = jxx::NEW<CoderResult>(jxx::NEW<jxx::lang::String>("UNDERFLOW"));
    jxx::Ptr<CoderResult> CoderResult::JXX_OVERFLOW = jxx::NEW<CoderResult>(jxx::NEW<jxx::lang::String>("OVERFLOW"));

    CoderResult::CoderResult(jxx::Ptr<jxx::lang::String> kind,
                             jxx::lang::jint length)
        : kind_(std::move(kind)), length_(length)
    {
    }

    jxx::Ptr<CoderResult> CoderResult::malformedForLength(jxx::lang::jint length)
    {
        return jxx::NEW<CoderResult>(jxx::NEW<jxx::lang::String>("MALFORMED"), length);
    }

    jxx::Ptr<CoderResult> CoderResult::unmappableForLength(jxx::lang::jint length)
    {
        return jxx::NEW<CoderResult>(jxx::NEW<jxx::lang::String>("UNMAPPABLE"), length);
    }

    jxx::lang::jbool CoderResult::isUnderflow() const { return kind_ && kind_->utf8() == "UNDERFLOW"; }
    jxx::lang::jbool CoderResult::isOverflow() const { return kind_ && kind_->utf8() == "OVERFLOW"; }
    jxx::lang::jbool CoderResult::isMalformed() const { return kind_ && kind_->utf8() == "MALFORMED"; }
    jxx::lang::jbool CoderResult::isUnmappable() const { return kind_ && kind_->utf8() == "UNMAPPABLE"; }
    jxx::lang::jbool CoderResult::isError() const { return isMalformed() || isUnmappable(); }
    jxx::lang::jint CoderResult::length() const { return length_; }
    jxx::Ptr<jxx::lang::String> CoderResult::toString() const { return kind_; }
}
