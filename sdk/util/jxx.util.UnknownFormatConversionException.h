#pragma once
#include "jxx.util.format.IllegalFormatException.h"

namespace jxx::util::format {

class UnknownFormatConversionException : public IllegalFormatException {
public:
    explicit UnknownFormatConversionException(jxx::Ptr<jxx::lang::String> msg)
        : IllegalFormatException(std::move(msg)) {}
};

} // namespace jxx::util::format
