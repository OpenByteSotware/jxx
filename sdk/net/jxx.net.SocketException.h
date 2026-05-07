#pragma once

#include <memory>
#include "jxx.io.IOException.h"

namespace jxx::net {

class SocketException : public jxx::io::IOException {
public:
    using jxx::io::IOException::IOException;
    JXX_OBJECT_CLONE(SocketException)
protected:
    const char* typeName() const noexcept override { return "SocketException"; }
};

} // namespace jxx::net