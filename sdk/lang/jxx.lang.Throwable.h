// jxx/lang/Throwable.h
#pragma once

#include <exception>
#include <memory>
#include <string>
#include <vector>
#include <ostream>

#include "jxx.lang.Object.h"
#include "jxx.lang.String.h"
#include "jxx.lang.StackTrace.h"  // StackTraceElement + captureStackTrace()

namespace jxx::lang {

    class IllegalArgumentException;  // you already have this
    // (later you can swap to NullPointerException for exact Java parity)

    class Throwable : public Object, public std::exception {
    public:
        using Ptr = std::shared_ptr<Throwable>;

        Throwable(String message = String(""),
            Ptr cause = nullptr,
            bool enableSuppression = true,
            bool writableStackTrace = true)
            : message_(std::move(message)),
            cause_(std::move(cause)),
            enableSuppression_(enableSuppression),
            writableStackTrace_(writableStackTrace) {
            if (writableStackTrace_) {
                stack_ = captureStackTrace(/*skipFrames=*/1);
            }
        }

        virtual ~Throwable() = default;

        // Java-like API
        const String& getMessage() const { return message_; }
        Ptr getCause() const { return cause_; }

        // Optional Java-ish initCause (Java has constraints; implement if you want strict parity)
        void initCause(Ptr cause) { cause_ = std::move(cause); }

        // ---- Suppressed exceptions (Java 7+, present in Java 8) ----
        void addSuppressed(const Ptr& ex) {
            if (!enableSuppression_) return;

            if (!ex) {
                // Java would throw NullPointerException; you don't have it yet.
                throw IllegalArgumentException(String("Cannot suppress null exception"));
            }
            if (ex.get() == this) {
                // Java: IllegalArgumentException for self-suppression
                throw IllegalArgumentException(String("Self-suppression not permitted"));
            }
            suppressed_.push_back(ex);
        }

        std::vector<Ptr> getSuppressed() const {
            // Java returns a copy of the array; vector copy matches semantics well
            return suppressed_;
        }

        // ---- Stack trace (Java-style) ----
        // Java: fillInStackTrace returns this
        Throwable& fillInStackTrace() {
            if (writableStackTrace_) {
                stack_ = captureStackTrace(/*skipFrames=*/1);
            }
            return *this;
        }

        const std::vector<StackTraceElement>& getStackTrace() const {
            return stack_;
        }

        // Java-like pretty printing
        void printStackTrace(std::ostream& os) const {
            os << typeName() << ": " << message_.toStdString() << "\n";

            for (const auto& e : stack_) {
                os << "\tat " << e.methodName;
                if (!e.fileName.empty()) {
                    os << "(" << e.fileName;
                    if (e.lineNumber >= 0) os << ":" << e.lineNumber;
                    os << ")";
                }
                os << " [0x" << std::hex << e.address << std::dec << "]\n";
            }

            for (const auto& s : suppressed_) {
                os << "Suppressed: ";
                if (s) s->printStackTrace(os);
            }

            if (cause_) {
                os << "Caused by: ";
                cause_->printStackTrace(os);
            }
        }

        // std::exception bridge
        const char* what() const noexcept override {
            // Cache is required because toStdString() returns a temporary
            cached_ = message_.toStdString();
            return cached_.c_str();
        }

        // crucial: avoids slicing when wrapping as shared_ptr<Throwable>
        virtual Ptr clone() const = 0;

        inline void addSuppressed(const Ptr& ex) {
            if (!enableSuppression_) return;

            if (!ex) {
                throw NullPointerException(String("Cannot suppress null exception"));
            }
            if (ex.get() == this) {
                throw IllegalArgumentException(String("Self-suppression not permitted"));
            }
            suppressed_.push_back(ex);
        }

    protected:
        // Override in derived exceptions to match Java class names if desired
        virtual const char* typeName() const noexcept { return "Throwable"; }

    private:
        String message_{ "" };
        Ptr cause_{ nullptr };

        bool enableSuppression_{ true };
        bool writableStackTrace_{ true };

        std::vector<Ptr> suppressed_;
        std::vector<StackTraceElement> stack_;

        mutable std::string cached_;
    };

} // namespace jxx::lang


#define JXX_THROWABLE_CLONE(Derived) \
    ::jxx::lang::Throwable::Ptr clone() const override { \
        return std::make_shared<Derived>(*this); \
    }
