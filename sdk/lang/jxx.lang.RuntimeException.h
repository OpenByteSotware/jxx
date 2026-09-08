#pragma once

#include <string>

#include "lang/jxx.lang.ClassInfoMarker.h"
#include "lang/jxx.lang.Exception.h"

namespace jxx::lang {

class ClassAny;
class String;
class Throwable;

template <
    typename Derived,
    typename JxxSuper,
    typename... JxxInterfaces>
class ClassInfo;

class RuntimeException
    : public Exception {
public:
    using JxxSuper = Exception;

    using JxxClassInfoMarker =
        ClassInfo<RuntimeException, JxxSuper>;

    static jxx::Ptr<ClassAny> Class();

    RuntimeException();

    explicit RuntimeException(
        const jxx::Ptr<String>& message);

    RuntimeException(
        const jxx::Ptr<String>& message,
        const jxx::Ptr<Throwable>& cause);

    explicit RuntimeException(
        const jxx::Ptr<Throwable>& cause);

    explicit RuntimeException(
        const char* message);

    explicit RuntimeException(
        const std::string& message);

    RuntimeException(
        const RuntimeException& other);

    RuntimeException(
        RuntimeException&& other) noexcept;

    RuntimeException& operator=(
        const RuntimeException& other);

    RuntimeException& operator=(
        RuntimeException&& other) noexcept;

    ~RuntimeException() override;

protected:
    jxx::Ptr<Object> cloneImpl() const override;
    const char* typeName() const noexcept override;
};

} // namespace jxx::lang
