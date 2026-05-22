#pragma once

#include <exception>
#include <memory>
#include <string>
#include <vector>
#include <ostream>
#include <utility>
#include "lang/jxx.lang.Object.h"
#include "lang/jxx.lang.StackTrace.h"

namespace jxx::lang {

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
    //void throwNullSuppressed();
    //void throwSelfSuppressed();

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
        explicit Throwable(jxx::Ptr<String> message = jxx::NEW<String>(""),
            jxx::Ptr<Throwable> cause = nullptr,
            bool enableSuppression = true,
            bool writableStackTrace = true);

        explicit Throwable(const char* message,
            jxx::Ptr<Throwable> cause = nullptr,
            bool enableSuppression = true,
            bool writableStackTrace = true);

        explicit Throwable(std::string message,
            jxx::Ptr<Throwable> cause = nullptr,
            bool enableSuppression = true,
            bool writableStackTrace = true);

        virtual ~Throwable() = default;

        // ---- Java-like message/cause API ----
        const jxx::Ptr<String>& getMessage() const;
        jxx::Ptr<Throwable> getCause() const;

        /**
         * Java's initCause has constraints (can only be set once, cannot set self, etc.).
         * This is a permissive version; tighten if you want exact rules.
         */
        void initCause(jxx::Ptr<Throwable> cause);

        // ---- Suppressed exceptions (Java 7+, present in Java 8) ----
        /**
         * Java rules:
         *   - addSuppressed(null) => NullPointerException
         *   - addSuppressed(this) => IllegalArgumentException
         *   - if suppression disabled => no-op
         */
        void addSuppressed(const jxx::Ptr<Throwable> ex);

        /**
         * Java returns a copy of the internal array.
         * Returning by value matches that semantic well.
         */
        std::vector<jxx::Ptr<Throwable>> getSuppressed() const;

        // ---- Stack trace (symbol-only, Windows+Linux) ----
        /**
         * Java: fillInStackTrace() captures and returns this.
         */
        Throwable& fillInStackTrace();

        const std::vector<StackTraceElement>& getStackTrace() const;

        /**
         * Java-like printing:
         *   ExceptionType: message
         *     at dotted.symbol(...) [0xADDR]
         * Suppressed + Cause chain are printed recursively.
         *
         * Note: StackTraceElement::symbol is already formatted by your StackTrace module
         * (dotted, template compression, operator() prettify, etc.).
         */
        void printStackTrace(std::ostream& os) const;

        // ---- Stringification ----
        /**
         * Java-like toString():
         *   ClassName
         *   ClassName: message   (when message is non-empty)
         *
         * Returns a stable const char* valid until the next toString() call
         * (or object destruction).
         */
        virtual jxx::Ptr<jxx::lang::String> toString() const;

        /**
         * std::exception bridge. Same caching rationale as toString().
         */
        const char* what() const noexcept override;

        // ---- Cloning ----
        /**
         * IMPORTANT: shared_ptr is not covariant, so this must match Object::clone().
         * All Throwable-derived exceptions should implement this using the macro below.
         */
        jxx::Ptr<jxx::lang::Object> cloneImpl() const override = 0;

        /**
         * Typed helper for exception cause wrapping:
         * returns shared_ptr<Throwable> while preserving dynamic type.
         */
        jxx::Ptr<Throwable> cloneThrowable() const;

    protected:
        /**
         * Override in derived exceptions for exact Java naming.
         * If you want max Java parity, return fully-qualified names like:
         *   "java.io.IOException"
         * Otherwise short names like "IOException" are fine.
         */
        virtual const char* typeName() const noexcept;

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
