#pragma once
#include "lang/jxx.lang.ClassInfoMarker.h"

#include <memory>
#include "lang/jxx_types.h"
#include "lang/jxx.lang.Object.h"
#include "lang/jxx.lang.buildin_array.h"
#include "jxx.util.IllegalFormatException.h"

namespace jxx::util {

class IllegalFormatPrecisionException : public jxx::util::IllegalFormatException {
public:
    using JxxSuper = jxx::util::IllegalFormatException;
    using JxxClassInfoMarker = jxx::lang::ClassInfo<IllegalFormatPrecisionException, JxxSuper>;

    static jxx::Ptr<jxx::lang::ClassAny> Class();

    IllegalFormatPrecisionException(const IllegalFormatPrecisionException&) = default;
    IllegalFormatPrecisionException(IllegalFormatPrecisionException&&) noexcept = default;
    IllegalFormatPrecisionException& operator=(const IllegalFormatPrecisionException&) = default;
    IllegalFormatPrecisionException& operator=(IllegalFormatPrecisionException&&) noexcept = default;
    ~IllegalFormatPrecisionException() override = default;

public:
    using jxx::util::IllegalFormatException::IllegalFormatException;
	IllegalFormatPrecisionException() = default;
    IllegalFormatPrecisionException(const jxx::Ptr<jxx::lang::String> msg)
		: jxx::util::IllegalFormatException(std::move(msg)) {}
    IllegalFormatPrecisionException(jxx::lang::jint precision)
		: jxx::util::IllegalFormatException(jxx::NEW<jxx::lang::String>("precision: " + std::to_string(precision))) {}   
    JXX_OBJECT_CLONE(IllegalFormatPrecisionException)
protected:
    const char* typeName() const noexcept override { return "IllegalFormatPrecisionException"; }
};

} // namespace jxx::util