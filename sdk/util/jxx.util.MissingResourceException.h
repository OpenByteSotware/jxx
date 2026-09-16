#pragma once

#include "lang/jxx.lang.ClassInfoMarker.h"
#include "lang/jxx.lang.RuntimeException.h"

namespace jxx::lang {
class String;
}

namespace jxx::util {

class MissingResourceException : public jxx::lang::RuntimeException {
public:
    using JxxSuper = jxx::lang::RuntimeException;
    using JxxClassInfoMarker =
        jxx::lang::ClassInfo<MissingResourceException, JxxSuper>;

    static jxx::Ptr<jxx::lang::ClassAny> Class();

    MissingResourceException(
        const jxx::Ptr<jxx::lang::String>& message,
        const jxx::Ptr<jxx::lang::String>& className,
        const jxx::Ptr<jxx::lang::String>& key);

    jxx::Ptr<jxx::lang::String> getClassName() const;
    jxx::Ptr<jxx::lang::String> getKey() const;

    MissingResourceException(const MissingResourceException&) = default;
    MissingResourceException(MissingResourceException&&) noexcept = default;
    MissingResourceException& operator=(const MissingResourceException&) = default;
    MissingResourceException& operator=(MissingResourceException&&) noexcept = default;
    ~MissingResourceException() override = default;

protected:
    JXX_OBJECT_CLONE(MissingResourceException)
    const char* typeName() const noexcept override;

private:
    jxx::Ptr<jxx::lang::String> className_;
    jxx::Ptr<jxx::lang::String> key_;
};

} // namespace jxx::util
