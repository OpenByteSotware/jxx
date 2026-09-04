#pragma once
#include "lang/jxx.lang.ClassInfoMarker.h"

#include <memory>
#include "util/jxx.util.IllegalFormatException.h"

namespace jxx::util {

    class DuplicateFormatFlagsException : public IllegalFormatException {
public:
    using JxxSuper = IllegalFormatException;
    using JxxClassInfoMarker = jxx::lang::ClassInfo<DuplicateFormatFlagsException, JxxSuper>;

    static jxx::Ptr<jxx::lang::ClassAny> Class();

    DuplicateFormatFlagsException() = default;

    DuplicateFormatFlagsException(const DuplicateFormatFlagsException&) = default;
    DuplicateFormatFlagsException(DuplicateFormatFlagsException&&) noexcept = default;
    DuplicateFormatFlagsException& operator=(const DuplicateFormatFlagsException&) = default;
    DuplicateFormatFlagsException& operator=(DuplicateFormatFlagsException&&) noexcept = default;
    ~DuplicateFormatFlagsException() override = default;

    public:
        using IllegalFormatException::IllegalFormatException;
        JXX_OBJECT_CLONE(DuplicateFormatFlagsException)
    protected:
        const char* typeName() const noexcept override { return "DuplicateFormatFlagsException"; }
    };

} // namespace jxx::util
