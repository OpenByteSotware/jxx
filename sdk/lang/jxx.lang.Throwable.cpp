#include "jxx.lang.String.h"
#include "jxx.lang.Throwable.h"
#include "jxx.lang.NullPointerException.h"
#include "jxx.lang.IllegalArgumentException.h"

namespace jxx::lang {

    Throwable::Throwable(jxx::Ptr<String> message,
        jxx::Ptr<Throwable> cause,
        bool enableSuppression,
        bool writableStackTrace)
        : message_(message != nullptr ? message : jxx::NEW<String>("")),
        cause_(cause),
        enableSuppression_(enableSuppression),
        writableStackTrace_(writableStackTrace) {
        if (writableStackTrace_) {
            // Java-like: capture stack at construction (best-effort)
            stack_ = captureStackTrace(/*skipFrames=*/1);
        }
    }

    Throwable::Throwable(const char* message,
        jxx::Ptr<Throwable> cause,
        bool enableSuppression,
        bool writableStackTrace)
        : message_(jxx::NEW<String>(message)),
        cause_(cause),
        enableSuppression_(enableSuppression),
        writableStackTrace_(writableStackTrace) {
        if (writableStackTrace_) {
            // Java-like: capture stack at construction (best-effort)
            stack_ = captureStackTrace(/*skipFrames=*/1);
        }
    }

    Throwable::Throwable(std::string message,
        jxx::Ptr<Throwable> cause,
        bool enableSuppression,
        bool writableStackTrace)
        : message_(jxx::NEW<String>(message)),
        cause_(cause),
        enableSuppression_(enableSuppression),
        writableStackTrace_(writableStackTrace) {
        if (writableStackTrace_) {
            // Java-like: capture stack at construction (best-effort)
            stack_ = captureStackTrace(/*skipFrames=*/1);
        }
    }

    // Copy constructor
    Throwable::Throwable(const Throwable& other)
        : Object(other),
        std::exception(other),
        message_(other.message_),
        cause_(other.cause_),
        enableSuppression_(other.enableSuppression_),
        writableStackTrace_(other.writableStackTrace_),
        suppressed_(other.suppressed_),
        stack_(other.stack_),
        cachedWhat_(other.cachedWhat_),
        cachedToString_(other.cachedToString_) {
    }

    // Move constructor
    Throwable::Throwable(Throwable&& other) noexcept
        : Object(std::move(other)),
        std::exception(std::move(other)),
        message_(std::move(other.message_)),
        cause_(std::move(other.cause_)),
        enableSuppression_(other.enableSuppression_),
        writableStackTrace_(other.writableStackTrace_),
        suppressed_(std::move(other.suppressed_)),
        stack_(std::move(other.stack_)),
        cachedWhat_(std::move(other.cachedWhat_)),
        cachedToString_(std::move(other.cachedToString_)) {
    }

    // Copy assignment
    Throwable& Throwable::operator=(const Throwable& other) {
        if (this != &other) {
            Object::operator=(other);
            std::exception::operator=(other);
            message_ = other.message_;
            cause_ = other.cause_;
            enableSuppression_ = other.enableSuppression_;
            writableStackTrace_ = other.writableStackTrace_;
            suppressed_ = other.suppressed_;
            stack_ = other.stack_;
            cachedWhat_ = other.cachedWhat_;
            cachedToString_ = other.cachedToString_;
        }
        return *this;
    }

    // Move assignment
    Throwable& Throwable::operator=(Throwable&& other) noexcept {
        if (this != &other) {
            Object::operator=(std::move(other));
            std::exception::operator=(std::move(other));
            message_ = std::move(other.message_);
            cause_ = std::move(other.cause_);
            enableSuppression_ = other.enableSuppression_;
            writableStackTrace_ = other.writableStackTrace_;
            suppressed_ = std::move(other.suppressed_);
            stack_ = std::move(other.stack_);
            cachedWhat_ = std::move(other.cachedWhat_);
            cachedToString_ = std::move(other.cachedToString_);
        }
        return *this;
    }

    // ---- Java-like message/cause API ----
    const jxx::Ptr<String>& Throwable::getMessage() const { return message_; }
    jxx::Ptr<Throwable> Throwable::getCause() const { return cause_; }

    /**
     * Java's initCause has constraints (can only be set once, cannot set self, etc.).
     * This is a permissive version; tighten if you want exact rules.
     */
    void Throwable::initCause(jxx::Ptr<Throwable> cause) { cause_ = cause; }

    /**
     * Java returns a copy of the internal array.
     * Returning by value matches that semantic well.
     */
    std::vector<jxx::Ptr<Throwable>> Throwable::getSuppressed() const { return suppressed_; }

    // ---- Stack trace (symbol-only, Windows+Linux) ----
    /**
     * Java: fillInStackTrace() captures and returns this.
     */
    Throwable& Throwable::fillInStackTrace() {
        if (writableStackTrace_) {
            stack_ = captureStackTrace(/*skipFrames=*/1);
        }
        return *this;
    }

    const std::vector<StackTraceElement>& Throwable::getStackTrace() const {
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
    void Throwable::printStackTrace(std::ostream& os) const {
        os << typeName() << ": " << message_->toString() << "\n";

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
    jxx::Ptr<jxx::lang::String> Throwable::toString() const {
        const std::string msg = message_->utf8();

        cachedToString_.clear();
        // Reserve reduces reallocations (optional)
        cachedToString_.reserve(std::strlen(typeName()) + 2 + msg.size());

        cachedToString_ += typeName();
        if (!msg.empty()) {
            cachedToString_ += ": ";
            cachedToString_ += msg;
        }
        return jxx::NEW<String>(cachedToString_);
    }

    /**
     * std::exception bridge. Same caching rationale as toString().
     */
    const char* Throwable::what() const noexcept {
        cachedWhat_ = message_->utf8();
        return cachedWhat_.c_str();
    }
      
    /**
     * Typed helper for exception cause wrapping:
     * returns shared_ptr<Throwable> while preserving dynamic type.
     */
    jxx::Ptr<Throwable> Throwable::cloneThrowable() const {
        return jxx::CAST<Throwable, jxx::lang::Object>(clone());
    }


 const char* Throwable::typeName() const noexcept { return "Throwable"; }


void Throwable::addSuppressed(const jxx::Ptr<Throwable> ex) {
    if (!enableSuppression_) {
        return;
    }
    if (!ex) {
        throw NullPointerException("Cannot suppress null exception");
    }
    if (ex.get() == this) {
        throw IllegalArgumentException("Self-suppression not permitted");
    }
    suppressed_.push_back(ex);
}

} // namespace jxx::lang