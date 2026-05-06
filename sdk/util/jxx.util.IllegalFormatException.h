#pragma once
#include "jxx.lang.RuntimeException.h"
#include "jxx.lang.String.h"

namespace jxx::util::format {

class IllegalFormatException : public jxx::lang::RuntimeException {
public:
    IllegalFormatException() = default;
    explicit IllegalFormatException(jxx::Ptr<jxx::lang::String> msg)
        : jxx::lang::RuntimeException(std::move(msg)) {}
};

} // namespace jxx::util::format
