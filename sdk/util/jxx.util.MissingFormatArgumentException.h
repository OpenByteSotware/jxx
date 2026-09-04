#pragma once
#include "lang/jxx.lang.ClassInfoMarker.h"
#include "jxx.util.IllegalFormatException.h"

namespace jxx::util {

class MissingFormatArgumentException : public IllegalFormatException {
public:
    using JxxSuper = IllegalFormatException;
    using JxxClassInfoMarker = jxx::lang::ClassInfo<MissingFormatArgumentException, JxxSuper>;

    static jxx::Ptr<jxx::lang::ClassAny> Class();

    MissingFormatArgumentException(const MissingFormatArgumentException&) = default;
    MissingFormatArgumentException(MissingFormatArgumentException&&) noexcept = default;
    MissingFormatArgumentException& operator=(const MissingFormatArgumentException&) = default;
    MissingFormatArgumentException& operator=(MissingFormatArgumentException&&) noexcept = default;
    ~MissingFormatArgumentException() override = default;

public:
    explicit MissingFormatArgumentException(const jxx::Ptr<jxx::lang::String> msg)
        : IllegalFormatException(std::move(msg)) {}
};

} // namespace jxx::util::format
