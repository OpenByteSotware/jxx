#pragma once

#include <exception>
#include <memory>
#include <string>
#include <vector>
#include <ostream>
#include <utility>
#include <exception>

#include "lang/jxx.lang.Object.h"
#include "lang/jxx.lang.StackTrace.h"

namespace jxx::lang {

    // Forward declarations for exact Java addSuppressed() semantics.
    // We do NOT include these headers here to avoid circular includes.
    class String;
    class NullPointerException;
    class IllegalArgumentException;

    /**
     * Helper throwers for addSuppressed() exact Java semantics.
     *
     * Implement these in a .cpp that includes:
     *   #include "jxx/lang/NullPointerException.h"
     *   #include "jxx/lang/IllegalArgumentException.h"
     *
     * This avoids header include cycles while still throwing the correct types.
     */
    void throwNullSuppressed();
    void throwSelfSuppressed();

    /**
     * Java-parity Throwable for C++17:
     *   - extends Object (Java parity)
     *   - supports cause chain and suppressed exceptions
     *   - captures stack trace at construction (best-effort, symbol-only)
     *   - provides toString() and what()
     *
     * NOTE about cloning:
     *   Object::clone() returns shared_ptr<Object>. shared_ptr is NOT covariant.
     *   Therefore Throwable::clone() also returns shared_ptr<Object>.
     *   Use cloneThrowable() when you specifically need shared_ptr<Throwable>.
     */
    class Throwable : public Object, public std::exception {
    public:
        //using Ptr = std::shared_ptr<Throwable>;

        /**
         * Java parity knobs (Throwable has a protected constructor in Java that can disable these):
         *   - enableSuppression: if false, addSuppressed is a no-op
         *   - writableStackTrace: if false, stack trace is empty and fillInStackTrace is a no-op
         */
        explicit Throwable(jxx::Ptr<String> message = JXX_NEW<String>(""),
            jxx::Ptr<Throwable> cause = nullptr,
            bool enableSuppression = true,
            bool writableStackTrace = true)
            : message_(message != nullptr ? message : JXX_NEW<String>("")),
            cause_(std::move(cause)),
            enableSuppression_(enableSuppression),
            writableStackTrace_(writableStackTrace) {
            if (writableStackTrace_) {
                // Java-like: capture stack at construction (best-effort)
                stack_ = captureStackTrace(/*skipFrames=*/1);
            }
        }

        explicit Throwable(const char* message,
            jxx::Ptr<Throwable> cause = nullptr,
            bool enableSuppression = true,
            bool writableStackTrace = true)
            : message_(JXX_NEW<String>(message)),
            cause_(std::move(cause)),
            enableSuppression_(enableSuppression),
            writableStackTrace_(writableStackTrace) {
            if (writableStackTrace_) {
                // Java-like: capture stack at construction (best-effort)
                stack_ = captureStackTrace(/*skipFrames=*/1);
            }
        }

        explicit Throwable(std::string message,
            jxx::Ptr<Throwable> cause = nullptr,
            bool enableSuppression = true,
            bool writableStackTrace = true)
            : message_(JXX_NEW<String>(message)),
            cause_(std::move(cause)),
            enableSuppression_(enableSuppression),
            writableStackTrace_(writableStackTrace) {
            if (writableStackTrace_) {
                // Java-like: capture stack at construction (best-effort)
                stack_ = captureStackTrace(/*skipFrames=*/1);
            }
        }

        virtual ~Throwable() = default;

        // ---- Java-like message/cause API ----
        const jxx::Ptr<String>& getMessage() const { return message_; }
        jxx::Ptr<Throwable> getCause() const { return cause_; }

        /**
         * Java's initCause has constraints (can only be set once, cannot set self, etc.).
         * This is a permissive version; tighten if you want exact rules.
         */
        void initCause(jxx::Ptr<Throwable> cause) { cause_ = std::move(cause); }

        // ---- Suppressed exceptions (Java 7+, present in Java 8) ----
        /**
         * Java rules:
         *   - addSuppressed(null) => NullPointerException
         *   - addSuppressed(this) => IllegalArgumentException
         *   - if suppression disabled => no-op
         */
        void addSuppressed(const jxx::Ptr<Throwable> ex) {
            if (!enableSuppression_) return;

            if (!ex) {
                throwNullSuppressed(); // throws NullPointerException
            }
            if (ex.get() == this) {
                throwSelfSuppressed(); // throws IllegalArgumentException
            }
            suppressed_.push_back(ex);
        }

        /**
         * Java returns a copy of the internal array.
         * Returning by value matches that semantic well.
         */
        std::vector<jxx::Ptr<Throwable>> getSuppressed() const { return suppressed_; }

        // ---- Stack trace (symbol-only, Windows+Linux) ----
        /**
         * Java: fillInStackTrace() captures and returns this.
         */
        Throwable& fillInStackTrace() {
            if (writableStackTrace_) {
                stack_ = captureStackTrace(/*skipFrames=*/1);
            }
            return *this;
        }

        const std::vector<StackTraceElement>& getStackTrace() const {
            return stack_;
        }

        /**
         * Java-like printing:
         *   ExceptionType: message
         *     at dotted.symbol(...) [0xADDR]
         * Suppressed + Cause chain are printed recursively.
         *
         * Note: StackTraceElement::symbol is already formatted by your StackTrace module
         * (dotted, template compression, operator() prettify, etc.).
         */
        void printStackTrace(std::ostream& os) const {
            os << typeName() << ": " << message_->toStdString() << "\n";

            for (const auto& e : stack_) {
                os << "\tat " << e.symbol
                    << " [0x" << std::hex << e.address << std::dec << "]\n";
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

        // ---- Stringification ----
        /**
         * Java-like toString():
         *   ClassName
         *   ClassName: message   (when message is non-empty)
         *
         * Returns a stable const char* valid until the next toString() call
         * (or object destruction).
         */
        virtual jxx::Ptr<String>  toString() const {
            const std::string msg = message_->toStdString();

            cachedToString_.clear();
            // Reserve reduces reallocations (optional)
            cachedToString_.reserve(std::strlen(typeName()) + 2 + msg.size());

            cachedToString_ += typeName();
            if (!msg.empty()) {
                cachedToString_ += ": ";
                cachedToString_ += msg;
            }
            return JXX_NEW<String>(cachedToString_);
        }

        /**
         * std::exception bridge. Same caching rationale as toString().
         */
        const char* what() const noexcept override {
            cachedWhat_ = message_->toStdString();
            return cachedWhat_.c_str();
        }

        // ---- Cloning ----
        /**
         * IMPORTANT: shared_ptr is not covariant, so this must match Object::clone().
         * All Throwable-derived exceptions should implement this using the macro below.
         */
        jxx::Ptr<Object> cloneImpl() const override = 0;

        /**
         * Typed helper for exception cause wrapping:
         * returns shared_ptr<Throwable> while preserving dynamic type.
         */
        jxx::Ptr<Throwable> cloneThrowable() const {
            return std::static_pointer_cast<Throwable>(clone());
        }

    protected:
        /**
         * Override in derived exceptions for exact Java naming.
         * If you want max Java parity, return fully-qualified names like:
         *   "java.io.IOException"
         * Otherwise short names like "IOException" are fine.
         */
        virtual const char* typeName() const noexcept { return "Throwable"; }

    private:
        jxx::Ptr<String> message_;
        jxx::Ptr<Throwable> cause_;

        bool enableSuppression_;
        bool writableStackTrace_;

        std::vector<jxx::Ptr<Throwable>> suppressed_;
        std::vector<StackTraceElement> stack_;

        // Separate caches so what() and toString() don't trample each other
        mutable std::string cachedWhat_;
        mutable std::string cachedToString_;
    };
}
