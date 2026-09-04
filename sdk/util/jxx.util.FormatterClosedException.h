#pragma once
#include "lang/jxx.lang.ClassInfoMarker.h"

#include <memory>
#include "lang/jxx.lang.IllegalStateException.h"

namespace jxx::util {

class FormatterClosedException : public jxx::lang::IllegalStateException {
public:
    using JxxSuper = jxx::lang::IllegalStateException;
    using JxxClassInfoMarker = jxx::lang::ClassInfo<FormatterClosedException, JxxSuper>;

    static jxx::Ptr<jxx::lang::ClassAny> Class();

    FormatterClosedException() = default;

    FormatterClosedException(const FormatterClosedException&) = default;
    FormatterClosedException(FormatterClosedException&&) noexcept = default;
    FormatterClosedException& operator=(const FormatterClosedException&) = default;
    FormatterClosedException& operator=(FormatterClosedException&&) noexcept = default;
    ~FormatterClosedException() override = default;

public:
    using jxx::lang::IllegalStateException::IllegalStateException;
    JXX_OBJECT_CLONE(FormatterClosedException)
protected:
    const char* typeName() const noexcept override { return "FormatterClosedException"; }
};

} // namespace jxx::util