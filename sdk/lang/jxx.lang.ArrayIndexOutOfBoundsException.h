#pragma once
#include "lang/jxx.lang.ClassInfoMarker.h"

#include <memory>
#include "lang/jxx.lang.IndexOutOfBoundsException.h"

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

    explicit ArrayIndexOutOfBoundsException(
        const ::jxx::Ptr<::jxx::lang::String>& message);

    explicit ArrayIndexOutOfBoundsException(const char* message);

    explicit ArrayIndexOutOfBoundsException(
        ::jxx::lang::jint index);
    
protected:
    JXX_OBJECT_CLONE(ArrayIndexOutOfBoundsException)
    const char* typeName() const noexcept override { return "ArrayIndexOutOfBoundsException"; }
};

} // namespace jxx::lang