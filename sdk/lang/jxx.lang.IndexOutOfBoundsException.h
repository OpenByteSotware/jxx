#pragma once
#include "lang/jxx.lang.ClassInfoMarker.h"

#include <memory>
#include "lang/jxx.lang.RuntimeException.h"

namespace jxx::lang {

class IndexOutOfBoundsException : public RuntimeException {
public:
    using JxxSuper = RuntimeException;
    using JxxClassInfoMarker = jxx::lang::ClassInfo<IndexOutOfBoundsException, JxxSuper>;

    static jxx::Ptr<jxx::lang::ClassAny> Class();

    IndexOutOfBoundsException() = default;

    IndexOutOfBoundsException(const IndexOutOfBoundsException&) = default;
    IndexOutOfBoundsException(IndexOutOfBoundsException&&) noexcept = default;
    IndexOutOfBoundsException& operator=(const IndexOutOfBoundsException&) = default;
    IndexOutOfBoundsException& operator=(IndexOutOfBoundsException&&) noexcept = default;
    ~IndexOutOfBoundsException() override = default;

public:
    using RuntimeException::RuntimeException;
    

protected:
    JXX_OBJECT_CLONE(IndexOutOfBoundsException)
    const char* typeName() const noexcept override { return "IndexOutOfBoundsException"; }
};

} // namespace jxx::lang