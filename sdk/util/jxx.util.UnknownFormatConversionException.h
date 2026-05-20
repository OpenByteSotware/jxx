#pragma once
#include "jxx.util.IllegalFormatException.h"

namespace jxx::util {

class UnknownFormatConversionException : public IllegalFormatException {
public:
    explicit UnknownFormatConversionException(jxx::Ptr<jxx::lang::String> msg)
        : IllegalFormatException(std::move(msg)) {}
};

} // namespace jxx::util::format
