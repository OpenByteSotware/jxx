#pragma once
#include "lang/jxx.lang.ClassInfoMarker.h"

#include "lang/jxx.lang.String.h"
#include "util/jxx.util.NoSuchElementException.h"

namespace jxx {
namespace util {

class InputMismatchException : public virtual NoSuchElementException {
public:
    using JxxSuper = NoSuchElementException;
    using JxxClassInfoMarker = jxx::lang::ClassInfo<InputMismatchException, JxxSuper>;

    static jxx::Ptr<jxx::lang::ClassAny> Class();

    InputMismatchException() = default;

    InputMismatchException(const InputMismatchException&) = default;
    InputMismatchException(InputMismatchException&&) noexcept = default;
    InputMismatchException& operator=(const InputMismatchException&) = default;
    InputMismatchException& operator=(InputMismatchException&&) noexcept = default;
    ~InputMismatchException() override = default;

public:
    using NoSuchElementException::NoSuchElementException;

protected:
    JXX_OBJECT_CLONE(InputMismatchException)

        const char* typeName() const noexcept override { return "InputMismatchException"; }
};

} // namespace util
} // namespace jxx
