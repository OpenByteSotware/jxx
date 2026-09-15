#pragma once

#include "lang/jxx.lang.ClassInfoMarker.h"
#include "lang/jxx.lang.IndexOutOfBoundsException.h"

namespace jxx::lang {

class StringIndexOutOfBoundsException
    : public IndexOutOfBoundsException {
public:
    using JxxSuper = IndexOutOfBoundsException;
    using JxxClassInfoMarker =
        ClassInfo<StringIndexOutOfBoundsException, JxxSuper>;

    static ::jxx::Ptr<ClassAny> Class();

    StringIndexOutOfBoundsException();
    explicit StringIndexOutOfBoundsException(
        const ::jxx::Ptr<String>& message);
    explicit StringIndexOutOfBoundsException(const char* message);
    explicit StringIndexOutOfBoundsException(jint index);

    StringIndexOutOfBoundsException(
        const StringIndexOutOfBoundsException&) = default;
    StringIndexOutOfBoundsException(
        StringIndexOutOfBoundsException&&) noexcept = default;
    StringIndexOutOfBoundsException& operator=(
        const StringIndexOutOfBoundsException&) = default;
    StringIndexOutOfBoundsException& operator=(
        StringIndexOutOfBoundsException&&) noexcept = default;
    ~StringIndexOutOfBoundsException() override = default;

protected:
    JXX_OBJECT_CLONE(StringIndexOutOfBoundsException)
    const char* typeName() const noexcept override;
};

} // namespace jxx::lang
