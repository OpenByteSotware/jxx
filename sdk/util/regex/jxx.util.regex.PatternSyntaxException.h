#pragma once

#include "lang/jxx.lang.String.h"
#include "lang/jxx.lang.IllegalArgumentException.h"

namespace jxx {
namespace util {
namespace regex {

class PatternSyntaxException final : public virtual jxx::lang::IllegalArgumentException {
public:
    using IllegalArgumentException::IllegalArgumentException;
    JXX_OBJECT_CLONE(PatternSyntaxException)
protected:
    const char* typeName() const noexcept override { return "PatternSyntaxException"; }
};

} // namespace regex
} // namespace util
} // namespace jxx
