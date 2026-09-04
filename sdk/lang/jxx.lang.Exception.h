#pragma once
#include "lang/jxx.lang.ClassInfoMarker.h"

#include <stdexcept>
#include <string>
#include "lang/jxx.lang.Throwable.h"

namespace jxx::lang {
    class Exception : public jxx::lang::Throwable {
public:
    using JxxSuper = jxx::lang::Throwable;
    using JxxClassInfoMarker = jxx::lang::ClassInfo<Exception, JxxSuper>;

    static jxx::Ptr<jxx::lang::ClassAny> Class();

    Exception() = default;

    Exception(const Exception&) = default;
    Exception(Exception&&) noexcept = default;
    Exception& operator=(const Exception&) = default;
    Exception& operator=(Exception&&) noexcept = default;
    ~Exception() override = default;

    public:
        using jxx::lang::Throwable::Throwable;
        
    protected:
        JXX_OBJECT_CLONE(jxx::lang::Exception)
        const char* typeName() const noexcept override { return "Exception"; }
    };

    class InvocationTargetException : public Exception {
public:
    using JxxSuper = Exception;
    using JxxClassInfoMarker = jxx::lang::ClassInfo<InvocationTargetException, JxxSuper>;

    static jxx::Ptr<jxx::lang::ClassAny> Class();

    InvocationTargetException(const InvocationTargetException&) = default;
    InvocationTargetException(InvocationTargetException&&) noexcept = default;
    InvocationTargetException& operator=(const InvocationTargetException&) = default;
    InvocationTargetException& operator=(InvocationTargetException&&) noexcept = default;
    ~InvocationTargetException() override = default;

    public:
        InvocationTargetException(const jxx::Ptr<String> message, jxx::Ptr<Throwable> cause)
            : Exception(message, cause) {}
        
    protected:
        JXX_OBJECT_CLONE(InvocationTargetException)
        const char* typeName() const noexcept override { return "InvocationTargetException"; }
    };
}
