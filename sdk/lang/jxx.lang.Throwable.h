#pragma once

#include <exception>
#include <memory>
#include <string>
#include <vector>
#include <ostream>

#include "jxx.lang.String.h"
#include "jxx.lang.Object.h"
#include "jxx.lang.StackTrace.h" // captureStackTrace + StackTraceElement

namespace jxx::lang {

    // Forward declarations for exact-Java addSuppressed semantics.
    // Implementations are in Throwable.cpp to avoid circular includes.
    class NullPointerException;
    class IllegalArgumentException;

    class Throwable : public Object, public std::exception {
    public:
        using Ptr = std::shared_ptr<Throwable>;

        // Java parity knobs:
        // - enableSuppression: if false, addSuppressed() is a no-op
        // - writableStackTrace: if false, stack trace remains empty and fillInStackTrace() is a no-op
        Throwable(String message = String(""),
            Ptr cause = nullptr,
            bool enableSuppression = true,
            bool writableStackTrace = true)
            : message_(std::move(message)),
            cause_(std::move(cause)),
            enableSuppression_(enableSuppression),
            writableStackTrace_(writableStackTrace) {
            if (writableStackTrace_) {
                // Java parity: capture at construction
                stack_ = captureStackTrace(/*skipFrames=*/1);
            }
        }

        virtual ~Throwable() = default;

        // Java-like API
        const String& getMessage() const { return message_; }
        Ptr getCause() const { return cause_; }

        // (If you want strict Java rules: initCause throws if cause already set, etc.)
        void initCause(Ptr cause) { cause_ = std::move(cause); }

        // ---- Suppressed exceptions (Java 7+, present in Java 8) ----
        // Java rules:
        //   addSuppressed(null)  -> NullPointerException
        //   addSuppressed(this)  -> IllegalArgumentException
        // If suppression disabled, this is a no-op.
        void addSuppressed(const Ptr& ex);

        // Java returns a copy/array; returning by value matches semantics well.
        std::vector<Ptr> getSuppressed() const { return suppressed_; }

        // ---- Stack trace (Java-like) ----
        // Java: fillInStackTrace() returns this
        Throwable& fillInStackTrace() {
            if (writableStackTrace_) {
                stack_ = captureStackTrace(/*skipFrames=*/1);
            }
            return *this;
        }

        const std::vector<StackTraceElement>& getStackTrace() const {
            return stack_;
        }

        // Java-like stack trace printing (symbol-only, dotted formatting is handled by StackTrace module)
        void printStackTrace(std::ostream& os) const {
            os << typeName() << ": " << message_.toStdString() << "\n";

            for (const auto& e : stack_) {
                os << "\tat " << e.symbol
                    << " [0x" << std::hex << e.address << std::dec << "]\n";
            }

            // Java prints suppressed exceptions nested
            for (const auto& s : suppressed_) {
                os << "Suppressed: ";
                if (s) s->printStackTrace(os);
            }

            // Java prints cause chain
            if (cause_) {
                os << "Caused by: ";
                cause_->printStackTrace(os);
            }
        }

        // std::exception bridge
        const char* what() const noexcept override {
            // Safe because toStdString() returns a temporary; we cache it.
            cached_ = message_.toStdString();
            return cached_.c_str();
        }

        // Required for InvocationTargetException(String, shared_ptr<Throwable>) without slicing
        virtual JXX_PTR(Object) clone() const = 0;

    protected:
        // Override in derived exceptions if you want exact class names
        virtual const char* typeName() const noexcept { return "Throwable"; }

    private:
        String message_;
        Ptr cause_;

        bool enableSuppression_;
        bool writableStackTrace_;

        std::vector<Ptr> suppressed_;
        std::vector<StackTraceElement> stack_;

        mutable std::string cached_;
    };

    // Helper macro for clone() when exceptions are copy-constructible (you confirmed they are)
#define JXX_THROWABLE_CLONE(Derived) \
    JXX_PTR(Object)cloneImpl() const override { \
        return std::make_shared<Derived>(*this); \
    }

} // namespace jxx::lang