#pragma once

#include <stdexcept>
#include "jxx_types.h"

namespace jxx::lang {
    
    struct AutoCloseable {
        virtual ~AutoCloseable() = default;

        /**
         * Compare this object with another object.
         * Must throw on nullptr to match Java semantics.
         */
        virtual void close() = 0;
  
    };

} // namespace jxx::lang