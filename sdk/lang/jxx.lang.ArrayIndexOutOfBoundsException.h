#pragma once
#include "lang/jxx.lang.ClassInfoMarker.h"

#include <memory>
#include "jxx.lang.IndexOutOfBoundsException.h"

namespace jxx::lang {

class ArrayIndexOutOfBoundsException : public IndexOutOfBoundsException {
public:
    using JxxSuper = IndexOutOfBoundsException;
    using JxxClassInfoMarker = jxx::lang::ClassInfo<ArrayIndexOutOfBoundsException, JxxSuper>;

    static jxx::Ptr<jxx::lang::ClassAny> Class();

    ArrayIndexOutOfBoundsException() = default;

    ArrayIndexOutOfBoundsException(const ArrayIndexOutOfBoundsException&) = default;
    ArrayIndexOutOfBoundsException(ArrayIndexOutOfBoundsException&&) noexcept = default;
    ArrayIndexOutOfBoundsException& operator=(const ArrayIndexOutOfBoundsException&) = default;
    ArrayIndexOutOfBoundsException& operator=(ArrayIndexOutOfBoundsException&&) noexcept = default;
    ~ArrayIndexOutOfBoundsException() override = default;

public:
    using IndexOutOfBoundsException::IndexOutOfBoundsException;
    
protected:
    JXX_OBJECT_CLONE(ArrayIndexOutOfBoundsException)
    const char* typeName() const noexcept override { return "ArrayIndexOutOfBoundsException"; }
};

} // namespace jxx::lang