#pragma once
#include "lang/jxx.lang.ClassInfoMarker.h"

#include <memory>
#include "lang/jxx.lang.Exception.h"

namespace jxx::util {

    class TooManyListenersException : public jxx::lang::Exception {
public:
    using JxxSuper = jxx::lang::Exception;
    using JxxClassInfoMarker = jxx::lang::ClassInfo<TooManyListenersException, JxxSuper>;

    static jxx::Ptr<jxx::lang::ClassAny> Class();

    TooManyListenersException() = default;

    TooManyListenersException(const TooManyListenersException&) = default;
    TooManyListenersException(TooManyListenersException&&) noexcept = default;
    TooManyListenersException& operator=(const TooManyListenersException&) = default;
    TooManyListenersException& operator=(TooManyListenersException&&) noexcept = default;
    ~TooManyListenersException() override = default;

    public:
        using Exception::Exception;
        JXX_OBJECT_CLONE(TooManyListenersException)
    protected:
        const char* typeName() const noexcept override { return "TooManyListenersException"; }
    };

} // namespace jxx::util
