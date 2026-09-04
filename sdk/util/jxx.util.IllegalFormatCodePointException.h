#pragma once
#include "lang/jxx.lang.ClassInfoMarker.h"

#include <memory>
#include "util/jxx.util.IllegalFormatException.h"

namespace jxx::util {

    class IllegalFormatCodePointException : public IllegalFormatException {
public:
    using JxxSuper = IllegalFormatException;
    using JxxClassInfoMarker = jxx::lang::ClassInfo<IllegalFormatCodePointException, JxxSuper>;

    static jxx::Ptr<jxx::lang::ClassAny> Class();

    IllegalFormatCodePointException() = default;

    IllegalFormatCodePointException(const IllegalFormatCodePointException&) = default;
    IllegalFormatCodePointException(IllegalFormatCodePointException&&) noexcept = default;
    IllegalFormatCodePointException& operator=(const IllegalFormatCodePointException&) = default;
    IllegalFormatCodePointException& operator=(IllegalFormatCodePointException&&) noexcept = default;
    ~IllegalFormatCodePointException() override = default;

    public:
        using IllegalFormatException::IllegalFormatException;
        JXX_OBJECT_CLONE(IllegalFormatCodePointException)
    protected:
        const char* typeName() const noexcept override { return "IllegalFormatCodePointException"; }
    };

} // namespace jxx::util
