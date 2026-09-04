#pragma once
#include "lang/jxx.lang.ClassInfoMarker.h"

#include <memory>
#include "lang/jxx.lang.RuntimeException.h"

namespace jxx::lang {

    class NullPointerException : public RuntimeException {
public:
    using JxxSuper = RuntimeException;
    using JxxClassInfoMarker = jxx::lang::ClassInfo<NullPointerException, JxxSuper>;

    static jxx::Ptr<jxx::lang::ClassAny> Class();

    NullPointerException() = default;

    NullPointerException(const NullPointerException&) = default;
    NullPointerException(NullPointerException&&) noexcept = default;
    NullPointerException& operator=(const NullPointerException&) = default;
    NullPointerException& operator=(NullPointerException&&) noexcept = default;
    ~NullPointerException() override = default;

    public:
        using RuntimeException::RuntimeException;
        
    protected:
        JXX_OBJECT_CLONE(NullPointerException)
        const char* typeName() const noexcept override { return "NullPointerException"; }
    };

} // namespace jxx::lang
