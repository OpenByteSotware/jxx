#pragma once

#include <memory>
#include "lang/jxx_types.h"
#include "lang/jxx.lang.Object.h"
#include "lang/jxx.lang.buildin_array.h"
#include "jxx.util.IllegalFormatException.h"

namespace jxx::util {

class IllegalFormatPrecisionException : public jxx::util::IllegalFormatException {
public:
    using jxx::util::IllegalFormatException::IllegalFormatException;
	IllegalFormatPrecisionException() = default;
    IllegalFormatPrecisionException(jxx::Ptr<jxx::lang::String> msg)
		: jxx::util::IllegalFormatException(std::move(msg)) {}
    IllegalFormatPrecisionException(jint precision)
		: jxx::util::IllegalFormatException(JXX_NEW<jxx::lang::String>("precision: " + std::to_string(precision))) {}   
    JXX_OBJECT_CLONE(IllegalFormatPrecisionException)
protected:
    const char* typeName() const noexcept override { return "IllegalFormatPrecisionException"; }
};

} // namespace jxx::util