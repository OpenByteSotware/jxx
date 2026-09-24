#pragma once

#include <memory>
#include "lang/jxx.lang.ClassInfo.h"
#include "lang/jxx.lang.RuntimeException.h"

namespace jxx::nio {

class BufferOverflowException : public jxx::lang::ClassBase<BufferOverflowException, jxx::lang::RuntimeException> {
public:
    using JxxSuper = jxx::lang::RuntimeException;
    using Super = jxx::lang::ClassBase<BufferOverflowException, JxxSuper>;
    using Super::Super;
    JXX_OBJECT_CLONE(BufferOverflowException)
protected:
    const char* typeName() const noexcept override;
};

} // namespace jxx::nio