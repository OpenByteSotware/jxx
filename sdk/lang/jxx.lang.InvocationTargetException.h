#pragma once

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

class InvocationTargetException
    : public Exception {
public:
    using JxxSuper = Exception;

    using JxxClassInfoMarker =
        ClassInfo<InvocationTargetException, JxxSuper>;

    static jxx::Ptr<ClassAny> Class();

    explicit InvocationTargetException(
        const jxx::Ptr<Throwable>& cause);

    InvocationTargetException(
        const jxx::Ptr<String>& message,
        const jxx::Ptr<Throwable>& cause);

    InvocationTargetException(
        const InvocationTargetException& other);

    InvocationTargetException(
        InvocationTargetException&& other) noexcept;

    InvocationTargetException& operator=(
        const InvocationTargetException& other);

    InvocationTargetException& operator=(
        InvocationTargetException&& other) noexcept;

    ~InvocationTargetException() override;

    jxx::Ptr<Throwable> getTargetException() const;

protected:
    jxx::Ptr<Object> cloneImpl() const override;
    const char* typeName() const noexcept override;
};

} // namespace jxx::lang
