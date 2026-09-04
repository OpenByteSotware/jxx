#pragma once

#include <exception>
#include <ostream>
#include <string>
#include <utility>
#include <vector>

#include "lang/jxx.lang.ClassInfoMarker.h"
#include "lang/jxx.lang.Object.h"
#include "lang/jxx.lang.StackTrace.h"

namespace jxx::lang
{

    class ClassAny;
    class String;

    class NullPointerException;
    class IllegalArgumentException;

    template <
        typename Derived,
        typename JxxSuper,
        typename... JxxInterfaces>
    class ClassInfo;

    /**
     * Root JXX throwable type.
     *
     * This is a bootstrap class and therefore inherits Object directly.
     * It does not include ClassInfo.h and does not inherit ClassBase.
     *
     * Metadata registration is implemented out of line after the type is
     * complete.
     */
    class Throwable
        : public Object
        , public std::exception
    {
    public:
        using JxxSuper = Object;

        using JxxClassInfoMarker =
            ClassInfo<
            Throwable,
            JxxSuper>;

        static jxx::Ptr<ClassAny> Class();

        /**
         * Creates an empty throwable with stack capture enabled.
         */
        Throwable();

        /**
         * Creates a throwable with a message and optional cause.
         */
        explicit Throwable(
            const jxx::Ptr<String>& message,
            const jxx::Ptr<Throwable>& cause = nullptr,
            jbool enableSuppression = true,
            jbool writableStackTrace = true);

        /**
         * Native string convenience overload.
         */
        explicit Throwable(
            const char* message,
            const jxx::Ptr<Throwable>& cause = nullptr,
            jbool enableSuppression = true,
            jbool writableStackTrace = true);

        /**
         * Native string convenience overload.
         */
        explicit Throwable(
            const std::string& message,
            const jxx::Ptr<Throwable>& cause = nullptr,
            jbool enableSuppression = true,
            jbool writableStackTrace = true);

        /**
         * Creates a shallow throwable copy from a JXX reference.
         */
        explicit Throwable(
            const jxx::Ptr<Throwable>& other);

        /**
         * C++ implementation copy support.
         *
         * Object identity and synchronization state are not copied.
         */
        Throwable(
            const Throwable& other);

        /**
         * C++ implementation move support.
         *
         * Object identity and synchronization state are not moved.
         */
        Throwable(
            Throwable&& other) noexcept;

        Throwable& operator=(
            const Throwable& other);

        Throwable& operator=(
            Throwable&& other) noexcept;

        ~Throwable() override = default;

        const jxx::Ptr<String>& getMessage() const;

        jxx::Ptr<Throwable> getCause() const;

        void initCause(
            const jxx::Ptr<Throwable>& cause);

        void addSuppressed(
            const jxx::Ptr<Throwable>& exception);

        /**
         * Transitional native representation.
         *
         * A later strict public-API migration can return a JxxArray reference
         * without changing the internal storage.
         */
        std::vector<jxx::Ptr<Throwable>>
            getSuppressed() const;

        Throwable& fillInStackTrace();

        /**
         * Transitional native representation.
         */
        const std::vector<StackTraceElement>&
            getStackTrace() const;

        void printStackTrace(
            std::ostream& output) const;

        jxx::Ptr<String> toString()
            const override;

        const char* what()
            const noexcept override;

        jxx::Ptr<Throwable>
            cloneThrowable() const;

    protected:
        jxx::Ptr<Object>
            cloneImpl() const override;

        virtual const char*
            typeName() const noexcept;

    private:
        jxx::Ptr<String> message_;
        jxx::Ptr<Throwable> cause_;

        jbool enableSuppression_ = true;
        jbool writableStackTrace_ = true;

        std::vector<jxx::Ptr<Throwable>>
            suppressed_;

        std::vector<StackTraceElement>
            stack_;

        mutable std::string cachedWhat_;
        mutable std::string cachedToString_;
    };

} // namespace jxx::lang