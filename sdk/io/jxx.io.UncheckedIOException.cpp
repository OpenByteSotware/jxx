#include "io/jxx.io.UncheckedIOException.h"

#include "lang/jxx.lang.ClassInfo.h"
#include "lang/jxx.lang.NullPointerException.h"
#include "lang/jxx.lang.String.h"

namespace jxx::io {

jxx::Ptr<jxx::lang::ClassAny> UncheckedIOException::Class()
{
    return JxxClassInfoMarker::Class();
}

UncheckedIOException::UncheckedIOException(const jxx::Ptr<IOException>& cause)
    : JxxSuper(cause == nullptr ? nullptr : cause->getMessage(), cause)
    , cause_(cause)
{
    if (cause == nullptr) {
        throw jxx::lang::NullPointerException();
    }
}

UncheckedIOException::UncheckedIOException(
    const jxx::Ptr<jxx::lang::String>& message,
    const jxx::Ptr<IOException>& cause)
    : JxxSuper(message, cause)
    , cause_(cause)
{
    if (cause == nullptr) {
        throw jxx::lang::NullPointerException();
    }
}

jxx::Ptr<IOException> UncheckedIOException::getCause() const
{
    return cause_;
}

jxx::Ptr<jxx::lang::Object> UncheckedIOException::cloneImpl() const
{
    return jxx::CAST<jxx::lang::Object>(jxx::NEW<UncheckedIOException>(*this));
}

const char* UncheckedIOException::typeName() const noexcept
{
    return "UncheckedIOException";
}

} // namespace jxx::io
