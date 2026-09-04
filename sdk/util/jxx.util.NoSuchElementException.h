#pragma once
#include "lang/jxx.lang.ClassInfoMarker.h"

#include <memory>
#include "lang/jxx.lang.RuntimeException.h"

namespace jxx::util {

class NoSuchElementException : public jxx::lang::RuntimeException {
public:
    using JxxSuper = jxx::lang::RuntimeException;
    using JxxClassInfoMarker = jxx::lang::ClassInfo<NoSuchElementException, JxxSuper>;

    static jxx::Ptr<jxx::lang::ClassAny> Class();

    NoSuchElementException() = default;

    NoSuchElementException(const NoSuchElementException&) = default;
    NoSuchElementException(NoSuchElementException&&) noexcept = default;
    NoSuchElementException& operator=(const NoSuchElementException&) = default;
    NoSuchElementException& operator=(NoSuchElementException&&) noexcept = default;
    ~NoSuchElementException() override = default;

public:
    using jxx::lang::RuntimeException::RuntimeException;
    JXX_OBJECT_CLONE(NoSuchElementException)
protected:
    const char* typeName() const noexcept override { return "NoSuchElementException"; }
};

} // namespace jxx::util