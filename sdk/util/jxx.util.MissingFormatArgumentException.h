#pragma once
#include "jxx.util.IllegalFormatException.h"

namespace jxx::util::format {

class MissingFormatArgumentException : public IllegalFormatException {
public:
    explicit MissingFormatArgumentException(jxx::Ptr<jxx::lang::String> msg)
        : IllegalFormatException(std::move(msg)) {}
};

} // namespace jxx::util::format
