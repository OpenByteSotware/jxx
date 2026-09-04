#pragma once
#include "lang/jxx.lang.ClassInfoMarker.h"

#include <memory>
#include "lang/jxx.lang.RuntimeException.h"

namespace jxx::util {

class ConcurrentModificationException : public jxx::lang::RuntimeException {
public:
    using JxxSuper = jxx::lang::RuntimeException;
    using JxxClassInfoMarker = jxx::lang::ClassInfo<ConcurrentModificationException, JxxSuper>;

    static jxx::Ptr<jxx::lang::ClassAny> Class();

    ConcurrentModificationException() = default;

    ConcurrentModificationException(const ConcurrentModificationException&) = default;
    ConcurrentModificationException(ConcurrentModificationException&&) noexcept = default;
    ConcurrentModificationException& operator=(const ConcurrentModificationException&) = default;
    ConcurrentModificationException& operator=(ConcurrentModificationException&&) noexcept = default;
    ~ConcurrentModificationException() override = default;

public:
    using jxx::lang::RuntimeException::RuntimeException;
    JXX_OBJECT_CLONE(ConcurrentModificationException)
protected:
    const char* typeName() const noexcept override { return "ConcurrentModificationException"; }
};

} // namespace jxx::util