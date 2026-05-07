#pragma once

#include <memory>
#include "jxx.io.IOException.h"

namespace jxx::net {

class MalformedURLException : public jxx::io::IOException {
public:
    using jxx::io::IOException::IOException;
    JXX_OBJECT_CLONE(MalformedURLException)
protected:
    const char* typeName() const noexcept override { return "MalformedURLException"; }
};

} // namespace jxx::net