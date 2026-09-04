#include "jxx.lang.Throwable.h"

#include <cstring>
#include <iomanip>
#include <utility>

#include "jxx.lang.ClassInfo.h"
#include "jxx.lang.IllegalArgumentException.h"
#include "jxx.lang.NullPointerException.h"
#include "jxx.lang.String.h"

namespace jxx::lang
{

    jxx::Ptr<ClassAny> Throwable::Class()
    {
        return JxxClassInfoMarker::Class();
    }

    Throwable::Throwable()
        : Object()
        , std::exception()
        , message_(jxx::NEW<String>(""))
        , cause_(nullptr)
        , enableSuppression_(true)
        , writableStackTrace_(true)
        , suppressed_()
        , stack_()
        , cachedWhat_()
        , cachedToString_()
    {
        stack_ = captureStackTrace(1);
    }

    Throwable::Throwable(
        const jxx::Ptr<String>& message,
        const jxx::Ptr<Throwable>& cause,
        jbool enableSuppression,
        jbool writableStackTrace)
        : Object()
        , std::exception()
        , message_(
              message != nullptr
                  ? message
                  : jxx::NEW<String>(""))
        , cause_(cause)
        , enableSuppression_(enableSuppression)
        , writableStackTrace_(writableStackTrace)
        , suppressed_()
        , stack_()
        , cachedWhat_()
        , cachedToString_()
    {
        if (writableStackTrace_) {
            stack_ = captureStackTrace(1);
        }
    }

    Throwable::Throwable(
        const char* message,
        const jxx::Ptr<Throwable>& cause,
        jbool enableSuppression,
        jbool writableStackTrace)
        : Throwable(
              jxx::NEW<String>(
                  message != nullptr
                  ? message
                  : ""),
              cause,
              enableSuppression,
              writableStackTrace)
    {
    }

    Throwable::Throwable(
        const std::string& message,
        const jxx::Ptr<Throwable>& cause,
        jbool enableSuppression,
        jbool writableStackTrace)
        : Throwable(
              jxx::NEW<String>(message),
              cause,
              enableSuppression,
              writableStackTrace)
    {
    }

    Throwable::Throwable(
        const jxx::Ptr<Throwable>& other)
        : Object()
        , std::exception()
        , message_(
              other != nullptr && other->message_ != nullptr
                  ? other->message_
                  : jxx::NEW<String>(""))
        , cause_(
              other != nullptr
                  ? other->cause_
                  : nullptr)
        , enableSuppression_(
              other != nullptr
                  ? other->enableSuppression_
                  : true)
        , writableStackTrace_(
              other != nullptr
                  ? other->writableStackTrace_
                  : true)
        , suppressed_(
              other != nullptr
                  ? other->suppressed_
                  : std::vector<jxx::Ptr<Throwable>>{})
        , stack_(
              other != nullptr
                  ? other->stack_
                  : std::vector<StackTraceElement>{})
        , cachedWhat_(
              other != nullptr
                  ? other->cachedWhat_
                  : std::string())
        , cachedToString_(
              other != nullptr
                  ? other->cachedToString_
                  : std::string())
    {
    }

    Throwable::Throwable(
        const Throwable& other)
        : Object()
        , std::exception(other)
        , message_(other.message_)
        , cause_(other.cause_)
        , enableSuppression_(other.enableSuppression_)
        , writableStackTrace_(other.writableStackTrace_)
        , suppressed_(other.suppressed_)
        , stack_(other.stack_)
        , cachedWhat_(other.cachedWhat_)
        , cachedToString_(other.cachedToString_)
    {
    }

    Throwable::Throwable(
        Throwable&& other) noexcept
        : Object()
        , std::exception(std::move(other))
        , message_(std::move(other.message_))
        , cause_(std::move(other.cause_))
        , enableSuppression_(other.enableSuppression_)
        , writableStackTrace_(other.writableStackTrace_)
        , suppressed_(std::move(other.suppressed_))
        , stack_(std::move(other.stack_))
        , cachedWhat_(std::move(other.cachedWhat_))
        , cachedToString_(std::move(other.cachedToString_))
    {
    }

    Throwable& Throwable::operator=(
        const Throwable& other)
    {
        if (this == &other) {
            return *this;
        }

        std::exception::operator=(other);

        message_ = other.message_;
        cause_ = other.cause_;
        enableSuppression_ = other.enableSuppression_;
        writableStackTrace_ = other.writableStackTrace_;
        suppressed_ = other.suppressed_;
        stack_ = other.stack_;
        cachedWhat_ = other.cachedWhat_;
        cachedToString_ = other.cachedToString_;

        return *this;
    }

    Throwable& Throwable::operator=(
        Throwable&& other) noexcept
    {
        if (this == &other) {
            return *this;
        }

        std::exception::operator=(std::move(other));

        message_ = std::move(other.message_);
        cause_ = std::move(other.cause_);
        enableSuppression_ = other.enableSuppression_;
        writableStackTrace_ = other.writableStackTrace_;
        suppressed_ = std::move(other.suppressed_);
        stack_ = std::move(other.stack_);
        cachedWhat_ = std::move(other.cachedWhat_);
        cachedToString_ = std::move(other.cachedToString_);

        return *this;
    }

    const jxx::Ptr<String>& Throwable::getMessage() const
    {
        return message_;
    }

    jxx::Ptr<Throwable> Throwable::getCause() const
    {
        return cause_;
    }

    void Throwable::initCause(
        const jxx::Ptr<Throwable>& cause)
    {
        if (cause.get() == this) {
            throw IllegalArgumentException(
                "Self-causation not permitted");
        }

        cause_ = cause;
    }

    void Throwable::addSuppressed(
        const jxx::Ptr<Throwable>& exception)
    {
        if (!enableSuppression_) {
            return;
        }

        if (exception == nullptr) {
            throw NullPointerException(
                "Cannot suppress a null exception");
        }

        if (exception.get() == this) {
            throw IllegalArgumentException(
                "Self-suppression not permitted");
        }

        suppressed_.push_back(exception);
    }

    std::vector<jxx::Ptr<Throwable>> Throwable::getSuppressed() const
    {
        return suppressed_;
    }

    Throwable& Throwable::fillInStackTrace()
    {
        if (writableStackTrace_) {
            stack_ = captureStackTrace(1);
        }

        return *this;
    }

    const std::vector<StackTraceElement>& Throwable::getStackTrace() const
    {
        return stack_;
    }

    void Throwable::printStackTrace(
        std::ostream& output) const
    {
        output << typeName();

        if (message_ != nullptr) {
            const auto messageText = message_->utf8();

            if (!messageText.empty()) {
                output << ": " << messageText;
            }
        }

        output << '\n';

        for (const auto& element : stack_) {
            output
                << "\tat "
                << element.symbol
                << " [0x"
                << std::hex
                << element.address
                << std::dec
                << "]\n";
        }

        for (const auto& exception : suppressed_) {
            output << "Suppressed: ";

            if (exception != nullptr) {
                exception->printStackTrace(output);
            }
            else {
                output << "null\n";
            }
        }

        if (cause_ != nullptr) {
            output << "Caused by: ";
            cause_->printStackTrace(output);
        }
    }

    jxx::Ptr<String> Throwable::toString() const
    {
        const std::string messageText =
            message_ != nullptr
            ? message_->utf8()
            : std::string();

        cachedToString_.clear();

        const char* const name = typeName();
        const std::size_t nameLength =
            name != nullptr
            ? std::strlen(name)
            : 0U;

        cachedToString_.reserve(
            nameLength +
            (messageText.empty() ? 0U : 2U) +
            messageText.size());

        if (name != nullptr) {
            cachedToString_ += name;
        }

        if (!messageText.empty()) {
            cachedToString_ += ": ";
            cachedToString_ += messageText;
        }

        return jxx::NEW<String>(cachedToString_);
    }

    const char* Throwable::what() const noexcept
    {
        try {
            cachedWhat_ =
                message_ != nullptr
                ? message_->utf8()
                : std::string();

            return cachedWhat_.c_str();
        }
        catch (...) {
            return "Throwable";
        }
    }

    jxx::Ptr<Throwable> Throwable::cloneThrowable() const
    {
        return jxx::CAST<Throwable>(clone());
    }

    const char* Throwable::typeName() const noexcept
    {
        return "Throwable";
    }

    jxx::Ptr<Object> Throwable::cloneImpl() const
    {
        auto result = jxx::NEW<Throwable>(*this);
        return jxx::CAST<Object>(result);
    }

} // namespace jxx::lang
