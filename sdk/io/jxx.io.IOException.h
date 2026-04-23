#pragma once

#include <memory>
#include "../lang/jxx.lang.Exception.h"
#include "../lang/jxx.lang.Throwable.h"

namespace jxx::io {

    class IOException : public jxx::lang::Exception {
    public:
        using CausePtr = std::shared_ptr<jxx::lang::Throwable>;

        // Java: public IOException()
        IOException()
            : jxx::lang::Exception(String(""), nullptr) {}

        // Java: public IOException(String message)
        explicit IOException(String message)
            : jxx::lang::Exception(std::move(message), nullptr) {}

        // Java: public IOException(String message, Throwable cause)
        IOException(String message, CausePtr cause)
            : jxx::lang::Exception(std::move(message), std::move(cause)) {}

        // Java: public IOException(Throwable cause)
        //
        // Java semantics: message = (cause == null ? null : cause.toString()).
        // In C++ we represent null as nullptr. If you add Throwable::toString(),
        // this matches Java closely.
        explicit IOException(CausePtr cause)
            : jxx::lang::Exception(causeMessageForJavaParity(cause), std::move(cause)) {}

        JXX_THROWABLE_CLONE(IOException)

    protected:
        const char* typeName() const noexcept override { return "IOException"; }

    private:
        static String causeMessageForJavaParity(const CausePtr& cause) {
            if (!cause) {
                return String(""); // Java message would be null; we represent as empty String
            }

            // BEST parity: if your Throwable exposes toString(), use it.
            // Uncomment this if you add it:
            //
            // return cause->toString();

            // Fallback: closest you can get without Throwable::toString():
            // Java toString() includes class name + ": " + message, but we at least preserve message.
            return cause->getMessage();
        }
    };

} // namespace jxx::io