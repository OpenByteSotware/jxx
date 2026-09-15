#pragma once

#include "lang/jxx.lang.ClassInfoMarker.h"
#include "lang/jxx.lang.RuntimeException.h"

namespace jxx::lang {

class TypeNotPresentException : public RuntimeException {
public:
    using JxxSuper = RuntimeException;
    using JxxClassInfoMarker = ClassInfo<TypeNotPresentException, JxxSuper>;

    static ::jxx::Ptr<ClassAny> Class();

    TypeNotPresentException(
        const ::jxx::Ptr<String>& typeName,
        const ::jxx::Ptr<Throwable>& cause);
    TypeNotPresentException(
        const char* typeName,
        const ::jxx::Ptr<Throwable>& cause);

    TypeNotPresentException(const TypeNotPresentException&) = default;
    TypeNotPresentException(TypeNotPresentException&&) noexcept = default;
    TypeNotPresentException& operator=(const TypeNotPresentException&) = default;
    TypeNotPresentException& operator=(TypeNotPresentException&&) noexcept = default;
    ~TypeNotPresentException() override = default;

    ::jxx::Ptr<String> getTypeName() const;

protected:
    JXX_OBJECT_CLONE(TypeNotPresentException)
    const char* typeName() const noexcept override;

private:
    ::jxx::Ptr<String> typeName_;
};

} // namespace jxx::lang
