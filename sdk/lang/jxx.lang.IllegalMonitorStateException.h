#pragma once
#include "lang/jxx.lang.ClassInfoMarker.h"

#include <memory>
#include "lang/jxx.lang.RuntimeException.h"

namespace jxx::lang {

    class IllegalMonitorStateException : public RuntimeException {
public:
    using JxxSuper = RuntimeException;
    using JxxClassInfoMarker = jxx::lang::ClassInfo<IllegalMonitorStateException, JxxSuper>;

    static jxx::Ptr<jxx::lang::ClassAny> Class();

    IllegalMonitorStateException() = default;

    IllegalMonitorStateException(const IllegalMonitorStateException&) = default;
    IllegalMonitorStateException(IllegalMonitorStateException&&) noexcept = default;
    IllegalMonitorStateException& operator=(const IllegalMonitorStateException&) = default;
    IllegalMonitorStateException& operator=(IllegalMonitorStateException&&) noexcept = default;
    ~IllegalMonitorStateException() override = default;

    public:
        using RuntimeException::RuntimeException;
        
    protected:
        JXX_OBJECT_CLONE(IllegalMonitorStateException)
        const char* typeName() const noexcept override { return "IllegalMonitorStateException"; }
    };

} // namespace jxx::lang
