#pragma once

#include <memory>
#include "jxx.lang.IllegalArgumentException.h"

namespace jxx::lang {

class NumberFormatException : public IllegalArgumentException {
public:
    using IllegalArgumentException::IllegalArgumentException;
    
protected:
	JXX_OBJECT_CLONE(NumberFormatException)

    const char* typeName() const noexcept override { return "NumberFormatException"; }
};

} // namespace jxx::lang
