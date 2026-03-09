#pragma once
#include <exception>
#include <string>
#include <vector>
#include <memory>
#include <ostream>
#include <sstream>
#include <typeinfo>
#include <utility>
#include "jxx.lang.Object.h"

#if defined(__GNUG__) || defined(__clang__)
  #include <cxxabi.h>
  #include <cstdlib>
#endif

namespace jxx::lang {

// -------------------- Source location --------------------
struct SourceLoc {
    const char* file{""};
    int         line{0};
    const char* func{""};
};

inline SourceLoc make_source_loc(const char* file, int line, const char* func) {
    return SourceLoc{file, line, func};
}


// -------------------- Throwable (base) --------------------
class Throwable : public Object, std::exception {
    std::string message_;
    std::exception_ptr cause_;
    std::vector<std::exception_ptr> suppressed_;
    std::vector<SourceLoc> trace_; // most-recent frame last

public:
    // Constructors mimic Java's combinations (message + cause). SourceLoc is required to capture frame.
    explicit Throwable(std::string msg,
                       std::exception_ptr cause,
                       SourceLoc where)
        : message_(std::move(msg)), cause_(std::move(cause)) {
        trace_.push_back(where);
    }

    // what()
    const char* what() const noexcept override { return message_.c_str(); }

    // Human name of dynamic type
    virtual std::string name() const {
        return demangle(typeid(*this).name());
    }

    // Message access
    const std::string& getMessage() const noexcept { return message_; }

    // Cause (nested exception)
    std::exception_ptr getCause() const noexcept { return cause_; }

    // Suppressed exceptions (like Java)
    void addSuppressed(std::exception_ptr e) { if (e) suppressed_.push_back(std::move(e)); }
    void addSuppressedCurrent() { addSuppressed(std::current_exception()); }
    const std::vector<std::exception_ptr>& getSuppressed() const noexcept { return suppressed_; }

    // Trace
    void appendTrace(SourceLoc where) { trace_.push_back(where); }
    const std::vector<SourceLoc>& trace() const noexcept { return trace_; }

    // toString()
    virtual std::string toString() const {
        if (message_.empty()) return name();
        return name() + ": " + message_;
    }

    // printStackTrace()
    virtual void printStackTrace(std::ostream& os) const {
        os << toString() << '\n';
        for (const auto& fr : trace_) {
            os << "  at " << fr.func << " (" << fr.file << ":" << fr.line << ")\n";
        }

        // Suppressed
        for (const auto& s : suppressed_) {
            os << "Suppressed: ";
            printExceptionPtr(s, os, 1);
        }

        // Cause
        if (cause_) {
            os << "Caused by: ";
            printExceptionPtr(cause_, os, 1);
        }
    }

    virtual ~Throwable() = default;

    // --------- Helpers to print std::exception_ptr chains ---------
    static void printStdException(const std::exception& e, std::ostream& os, int indent = 0) {
        const auto* t = dynamic_cast<const Throwable*>(&e);
        std::string pad(static_cast<size_t>(indent) * 2, ' ');
        if (t) {
            std::ostringstream sub;
            t->printStackTrace(sub);
            // Indent all lines
            std::istringstream in(sub.str());
            for (std::string line; std::getline(in, line); ) {
                os << pad << line << '\n';
            }
            return;
        }
        // Fallback for non-Throwable exceptions
        os << pad << demangle(typeid(e).name()) << ": " << e.what() << '\n';
        // Try to detect nested std::nested_exception (if used elsewhere)
        try {
            std::rethrow_if_nested(e);
        } catch (const std::exception& nested) {
            os << pad << "Caused by: ";
            printStdException(nested, os, indent + 1);
        } catch (...) {
            os << pad << "Caused by: <non-std::exception>\n";
        }
    }

    static void printExceptionPtr(const std::exception_ptr& ep, std::ostream& os, int indent = 0) {
        try {
            if (ep) std::rethrow_exception(ep);
        } catch (const std::exception& e) {
            printStdException(e, os, indent);
        } catch (...) {
            std::string pad(static_cast<size_t>(indent) * 2, ' ');
            os << pad << "<non-std::exception>\n";
        }
    }
};

// -------------------- Exception hierarchy --------------------
class Exception : public Throwable {
public:
    using Throwable::Throwable; // inherit ctors (message, cause, loc)
    ~Exception() override = default;
};

// A few illustrative specific types (mirror Java names)
class IOException : public Exception {
public: using Exception::Exception; ~IOException() override = default;
};

// -------------------- Macros for source loc --------------------
#define JXX_SOURCE_LOC ::jxx::lang::make_source_loc(__FILE__, __LINE__, __func__)

// Throw new exception with message
#define JXX_THROW(EXC_TYPE, MESSAGE) \
    throw EXC_TYPE{ (MESSAGE), std::exception_ptr{}, JXX_SOURCE_LOC }

// Throw with cause (must be an exception_ptr, often std::current_exception())
#define JXX_THROW_CAUSE(EXC_TYPE, MESSAGE, CAUSE_EP) \
    throw EXC_TYPE{ (MESSAGE), (CAUSE_EP), JXX_SOURCE_LOC }

// Rethrow an existing Throwable e, appending current context
#define JXX_RETHROW_APPEND(e) \
    do { (e).appendTrace(JXX_SOURCE_LOC); throw; } while(false)

} // namespace jxx::lang