#pragma once

#include <memory>
#include "lang/jxx.lang.ClassInfo.h"
#include "lang/jxx.lang.RuntimeException.h"

namespace jxx::nio {

class InvalidMarkException : public jxx::lang::ClassBase<InvalidMarkException, jxx::lang::RuntimeException> {
public:
    using JxxSuper = jxx::lang::RuntimeException;
    using Super = jxx::lang::ClassBase<InvalidMarkException, JxxSuper>;
    using Super::Super;
    JXX_OBJECT_CLONE(InvalidMarkException)
protected:
    const char* typeName() const noexcept override;
};

} // namespace jxx::nio