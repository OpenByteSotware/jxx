#pragma once
#include "lang/jxx.lang.ClassInfoMarker.h"

#include <memory>
#include "lang/jxx.lang.IllegalArgumentException.h"

namespace jxx::lang {

class NumberFormatException : public IllegalArgumentException {
public:
    using JxxSuper = IllegalArgumentException;
    using JxxClassInfoMarker = jxx::lang::ClassInfo<NumberFormatException, JxxSuper>;

    static jxx::Ptr<jxx::lang::ClassAny> Class();

    NumberFormatException() = default;

    NumberFormatException(const NumberFormatException&) = default;
    NumberFormatException(NumberFormatException&&) noexcept = default;
    NumberFormatException& operator=(const NumberFormatException&) = default;
    NumberFormatException& operator=(NumberFormatException&&) noexcept = default;
    ~NumberFormatException() override = default;

public:

    explicit NumberFormatException(
        const ::jxx::Ptr<::jxx::lang::String>& message);

    explicit NumberFormatException(const char* message);
    
protected:
	JXX_OBJECT_CLONE(NumberFormatException)

    const char* typeName() const noexcept override;
};

} // namespace jxx::lang
