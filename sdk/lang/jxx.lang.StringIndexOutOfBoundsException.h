#pragma once
#include "lang/jxx.lang.ClassInfoMarker.h"

#include <memory>
#include "jxx.lang.IndexOutOfBoundsException.h"

namespace jxx::lang {

class StringIndexOutOfBoundsException : public IndexOutOfBoundsException {
public:
    using JxxSuper = IndexOutOfBoundsException;
    using JxxClassInfoMarker = jxx::lang::ClassInfo<StringIndexOutOfBoundsException, JxxSuper>;

    static jxx::Ptr<jxx::lang::ClassAny> Class();

    StringIndexOutOfBoundsException() = default;

    StringIndexOutOfBoundsException(const StringIndexOutOfBoundsException&) = default;
    StringIndexOutOfBoundsException(StringIndexOutOfBoundsException&&) noexcept = default;
    StringIndexOutOfBoundsException& operator=(const StringIndexOutOfBoundsException&) = default;
    StringIndexOutOfBoundsException& operator=(StringIndexOutOfBoundsException&&) noexcept = default;
    ~StringIndexOutOfBoundsException() override = default;

public:
    using IndexOutOfBoundsException::IndexOutOfBoundsException;
    JXX_OBJECT_CLONE(StringIndexOutOfBoundsException)
protected:
    const char* typeName() const noexcept override { return "StringIndexOutOfBoundsException"; }
};

} // namespace jxx::lang
