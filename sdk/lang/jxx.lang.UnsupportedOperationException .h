#include <stdexcept>
#include <string>
#include "jxx.lang.RuntimeException.h"

namespace jxx::lang {
    // Custom exception class mirroring java.lang.UnsupportedOperationException
    class UnsupportedOperationException : public RuntimeException {
    public:
        // Constructor accepting string message
        explicit UnsupportedOperationException(const std::string& message)
            : std::logic_error(message) {}

        // Constructor accepting const char* message (for convenience)
        explicit UnsupportedOperationException(const char* message)
            : std::logic_error(message) {}
    };
}
