#pragma once
#include "lang/jxx.lang.IllegalArgumentException.h"


namespace jxx::lang {
    class String;
}

namespace jxx::util {

class IllegalFormatException : public jxx::lang::IllegalArgumentException {
public:
    IllegalFormatException() = default;
    explicit IllegalFormatException(jxx::Ptr<jxx::lang::String> msg)
        : jxx::lang::IllegalArgumentException(std::move(msg)) {}
};

} // namespace jxx::util::format
