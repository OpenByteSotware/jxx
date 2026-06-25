#pragma once

#include "lang/jxx.lang.String.h"
#include "util/jxx.util.NoSuchElementException.h"

namespace jxx {
namespace util {

class InputMismatchException : public virtual NoSuchElementException {
public:
    using NoSuchElementException::NoSuchElementException;

protected:
    JXX_OBJECT_CLONE(InputMismatchException)

        const char* typeName() const noexcept override { return "InputMismatchException"; }
};

} // namespace util
} // namespace jxx
