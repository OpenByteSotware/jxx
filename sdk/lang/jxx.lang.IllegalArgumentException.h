#pragma once

#include <string>

#include "lang/jxx.lang.ClassInfoMarker.h"
#include "lang/jxx.lang.RuntimeException.h"

namespace jxx::lang {

class ClassAny;
class String;
class Throwable;

template <
    typename Derived,
    typename JxxSuper,
    typename... JxxInterfaces>
class ClassInfo;

class IllegalArgumentException
    : public RuntimeException {
public:
    using JxxSuper = RuntimeException;

    using JxxClassInfoMarker =
        ClassInfo<IllegalArgumentException, JxxSuper>;

    static jxx::Ptr<ClassAny> Class();

    IllegalArgumentException();

    explicit IllegalArgumentException(
        const jxx::Ptr<String>& message);

    IllegalArgumentException(
        const jxx::Ptr<String>& message,
        const jxx::Ptr<Throwable>& cause);

    explicit IllegalArgumentException(
        const jxx::Ptr<Throwable>& cause);

    explicit IllegalArgumentException(
        const char* message);

    explicit IllegalArgumentException(
        const std::string& message);

    IllegalArgumentException(
        const IllegalArgumentException& other);

    IllegalArgumentException(
        IllegalArgumentException&& other) noexcept;

    IllegalArgumentException& operator=(
        const IllegalArgumentException& other);

    IllegalArgumentException& operator=(
        IllegalArgumentException&& other) noexcept;

    ~IllegalArgumentException() override;

protected:
    jxx::Ptr<Object> cloneImpl() const override;
    const char* typeName() const noexcept override;
};

} // namespace jxx::lang
