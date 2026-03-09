#include <stdexcept>
#include <string>
#include "jxx.lang.RuntimeException.h"

namespace jxx::lang {
    class IllegalStateException : public RuntimeException {
    public:
    public: using RuntimeException::RuntimeException; ~IllegalStateException() override = default;
    };
}
