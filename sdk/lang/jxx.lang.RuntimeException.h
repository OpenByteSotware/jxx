#pragma once
#include <exception>
#include <string>
#include <memory>

namespace jxx::lang {
    class RuntimeException : public Exception {
    public:
        using Exception::Exception;
        ~RuntimeException() override = default;

         // What(): returns the message (Java semantics)
        const char* what() const noexcept override {
            return message_.c_str();
        }

        // Equivalent to Java: getCause()
        std::exception_ptr getCause() const noexcept {
            return cause_;
        }

    private:
        std::string message_;
        std::exception_ptr cause_;
    };
}