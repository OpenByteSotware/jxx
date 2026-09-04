#pragma once
#include "lang/jxx.lang.ClassInfoMarker.h"

#include <memory>
#include "lang/jxx.lang.RuntimeException.h"

namespace jxx::lang {

    class ArrayStoreException : public RuntimeException {
public:
    using JxxSuper = RuntimeException;
    using JxxClassInfoMarker = jxx::lang::ClassInfo<ArrayStoreException, JxxSuper>;

    static jxx::Ptr<jxx::lang::ClassAny> Class();

    ArrayStoreException() = default;

    ArrayStoreException(const ArrayStoreException&) = default;
    ArrayStoreException(ArrayStoreException&&) noexcept = default;
    ArrayStoreException& operator=(const ArrayStoreException&) = default;
    ArrayStoreException& operator=(ArrayStoreException&&) noexcept = default;
    ~ArrayStoreException() override = default;

    public:
        using RuntimeException::RuntimeException;
        
    protected:
        JXX_OBJECT_CLONE(ArrayStoreException)
        const char* typeName() const noexcept override { return "ArrayStoreException"; }
    };

} // namespace jxx::lang
