#include <stdexcept>
#include <string>
#include "jxx.lang.RuntimeException.h"

namespace jxx::lang {
    // Semantically equivalent to java.lang.NullPointerException
    class NullPointerException : public RuntimeException {
    public:
    public: using RuntimeException::RuntimeException; ~NullPointerException() override = default;
    };
}
