#pragma once

#include "lang/jxx.lang.ClassInfoMarker.h"
#include "lang/jxx.lang.RuntimeException.h"

namespace jxx::lang {

class ClassAny;
class Object;
class String;

template <
    typename Derived,
    typename JxxSuper,
    typename... JxxInterfaces>
class ClassInfo;

class EmptyStackException
    : public RuntimeException {
public:
    using JxxSuper = RuntimeException;

    using JxxClassInfoMarker =
        ClassInfo<EmptyStackException, JxxSuper>;

    static ::jxx::Ptr<ClassAny> Class();

    EmptyStackException();
    EmptyStackException(const EmptyStackException& other);
    EmptyStackException(EmptyStackException&& other) noexcept;

    EmptyStackException& operator=(
        const EmptyStackException& other);

    EmptyStackException& operator=(
        EmptyStackException&& other) noexcept;

    ~EmptyStackException() override;

protected:
    ::jxx::Ptr<Object> cloneImpl() const override;
    const char* typeName() const noexcept override;
};

} // namespace jxx::lang
