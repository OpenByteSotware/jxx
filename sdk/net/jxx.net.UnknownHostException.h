#pragma once

#include <memory>
#include "jxx.io.IOException.h"

namespace jxx::net {

class UnknownHostException : public jxx::io::IOException {
public:
    using jxx::io::IOException::IOException;
    JXX_THROWABLE_CLONE(UnknownHostException)
protected:
    const char* typeName() const noexcept override { return "UnknownHostException"; }
};

} // namespace jxx::net