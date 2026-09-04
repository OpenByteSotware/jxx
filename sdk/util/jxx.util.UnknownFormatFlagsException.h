#pragma once
#include "lang/jxx.lang.ClassInfoMarker.h"

#include <memory>
#include "util/jxx.util.IllegalFormatException.h"

namespace jxx::util {

    class UnknownFormatFlagsException : public IllegalFormatException {
public:
    using JxxSuper = IllegalFormatException;
    using JxxClassInfoMarker = jxx::lang::ClassInfo<UnknownFormatFlagsException, JxxSuper>;

    static jxx::Ptr<jxx::lang::ClassAny> Class();

    UnknownFormatFlagsException() = default;

    UnknownFormatFlagsException(const UnknownFormatFlagsException&) = default;
    UnknownFormatFlagsException(UnknownFormatFlagsException&&) noexcept = default;
    UnknownFormatFlagsException& operator=(const UnknownFormatFlagsException&) = default;
    UnknownFormatFlagsException& operator=(UnknownFormatFlagsException&&) noexcept = default;
    ~UnknownFormatFlagsException() override = default;

    public:
        using IllegalFormatException::IllegalFormatException;
        JXX_OBJECT_CLONE(UnknownFormatFlagsException)
    protected:
        const char* typeName() const noexcept override { return "UnknownFormatFlagsException"; }
    };

} // namespace jxx::util
