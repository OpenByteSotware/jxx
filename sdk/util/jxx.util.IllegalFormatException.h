#pragma once
#include "lang/jxx.lang.ClassInfoMarker.h"
#include "lang/jxx.lang.IllegalArgumentException.h"


namespace jxx::lang {
    class String;
}

namespace jxx::util {

class IllegalFormatException : public jxx::lang::IllegalArgumentException {
public:
    using JxxSuper = jxx::lang::IllegalArgumentException;
    using JxxClassInfoMarker = jxx::lang::ClassInfo<IllegalFormatException, JxxSuper>;

    static jxx::Ptr<jxx::lang::ClassAny> Class();

    IllegalFormatException(const IllegalFormatException&) = default;
    IllegalFormatException(IllegalFormatException&&) noexcept = default;
    IllegalFormatException& operator=(const IllegalFormatException&) = default;
    IllegalFormatException& operator=(IllegalFormatException&&) noexcept = default;
    ~IllegalFormatException() override = default;

public:
    IllegalFormatException() = default;
    explicit IllegalFormatException(const jxx::Ptr<jxx::lang::String> msg)
        : jxx::lang::IllegalArgumentException(std::move(msg)) {}
};

} // namespace jxx::util::format
