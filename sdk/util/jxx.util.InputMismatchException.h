#pragma once

#include "lang/jxx.lang.String.h"
#include "util/jxx.util.NoSuchElementException.h"

namespace jxx {
namespace util {

class InputMismatchException : public virtual NoSuchElementException {
public:
    InputMismatchException();
    explicit InputMismatchException(jxx::Ptr<jxx::lang::String> message);
    virtual ~InputMismatchException() = default;
};

} // namespace util
} // namespace jxx
