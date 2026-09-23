#include <cstring>
#include <iomanip>
#include <utility>
#include "lang/jxx.lang.Throwable.h"
#include "lang/jxx.lang.ClassInfo.h"
#include "lang/jxx.lang.IllegalArgumentException.h"
#include "lang/jxx.lang.NullPointerException.h"
#include "lang/jxx.lang.String.h"
#include "lang/jxx.lang.StackTraceElement.h"

namespace jxx::lang
{

    namespace {

    std::vector<::jxx::Ptr<StackTraceElement>> capturePublicStackTrace(
        std::size_t skipFrames) {
        std::vector<::jxx::Ptr<StackTraceElement>> result;
        const auto nativeFrames = captureStackTrace(skipFrames + 1);
        result.reserve(nativeFrames.size());

        for (const auto& frame : nativeFrames) {
            result.push_back(
                ::jxx::NEW<StackTraceElement>(
                    ::jxx::NEW<String>(frame.symbol.empty() ? "unknown" : frame.symbol),
                    ::jxx::NEW<String>("unknown"),
                    nullptr,
                    -1));
        }
        return result;
    }

    } // namespace

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
        stack_ = capturePublicStackTrace(1);
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
            stack_ = capturePublicStackTrace(1);
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
                  : std::vector<::jxx::Ptr<StackTraceElement>>{})
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

    Throwable::SuppressedArray Throwable::getSuppressed() const
    {
        auto result = ::jxx::NEW<SuppressedArrayType>(
            static_cast<jint>(suppressed_.size()));
        for (std::size_t index = 0; index < suppressed_.size(); ++index) {
            (*result)[static_cast<jint>(index)] = suppressed_[index];
        }
        return result;
    }

    Throwable& Throwable::fillInStackTrace()
    {
        if (writableStackTrace_) {
            stack_ = capturePublicStackTrace(1);
        }

        return *this;
    }

    Throwable::StackTraceArray Throwable::getStackTrace() const
    {
        auto result = ::jxx::NEW<StackTraceArrayType>(
            static_cast<jint>(stack_.size()));

        for (std::size_t index = 0; index < stack_.size(); ++index) {
            (*result)[static_cast<jint>(index)] = stack_[index];
        }
        return result;
    }

    void Throwable::setStackTrace(
        const StackTraceArray& stackTrace)
    {
        if (stackTrace == nullptr) {
            throw NullPointerException();
        }

        std::vector<::jxx::Ptr<StackTraceElement>> replacement;
        replacement.reserve(stackTrace->length);
        for (jint index = 0; index < static_cast<jint>(stackTrace->length); ++index) {
            const auto element = (*stackTrace)[index];
            if (element == nullptr) {
                throw NullPointerException();
            }
            replacement.push_back(element);
        }
        stack_ = std::move(replacement);
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
            output << "\tat ";
            if (element != nullptr) {
                output << element->toString()->utf8();
            }
            output << '\n';
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
