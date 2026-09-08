#pragma once

#include <string>

#include "lang/jxx.lang.ClassInfoMarker.h"
#include "lang/jxx.lang.Throwable.h"

namespace jxx::lang {

class ClassAny;
class String;

template <
    typename Derived,
    typename JxxSuper,
    typename... JxxInterfaces>
class ClassInfo;

class Exception
    : public Throwable {
public:
    using JxxSuper = Throwable;

    using JxxClassInfoMarker =
        ClassInfo<Exception, JxxSuper>;

    static jxx::Ptr<ClassAny> Class();

    Exception();

    explicit Exception(
        const jxx::Ptr<String>& message);

    Exception(
        const jxx::Ptr<String>& message,
        const jxx::Ptr<Throwable>& cause);

    explicit Exception(
        const jxx::Ptr<Throwable>& cause);

    explicit Exception(
        const char* message);

    explicit Exception(
        const std::string& message);

    Exception(const Exception& other);
    Exception(Exception&& other) noexcept;

    Exception& operator=(
        const Exception& other);

    Exception& operator=(
        Exception&& other) noexcept;

    ~Exception() override;

protected:
    jxx::Ptr<Object> cloneImpl() const override;
    const char* typeName() const noexcept override;
};

} // namespace jxx::lang
