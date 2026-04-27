#pragma once

#include <memory>
#include "../lang/jxx.lang.Exception.h"

namespace jxx::io {

    class IOException : public jxx::lang::Exception {
    public:
        using jxx::lang::Exception::Exception;
        JXX_OBJECT_CLONE(IOException)

    protected:
        const char* typeName() const noexcept override { return "IOException"; }   
    };

} // namespace jxx::io