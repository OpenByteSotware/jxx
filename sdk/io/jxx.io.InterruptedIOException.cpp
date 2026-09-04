#include "io/jxx.io.InterruptedIOException.h"

#include "lang/jxx.lang.ClassInfo.h"

namespace jxx::io {

jxx::Ptr<jxx::lang::ClassAny> InterruptedIOException::Class()
{
    return JxxClassInfoMarker::Class();
}

InterruptedIOException::InterruptedIOException()
    : JxxSuper()
{
}

InterruptedIOException::InterruptedIOException(const jxx::Ptr<jxx::lang::String>& message)
    : JxxSuper(message)
{
}

InterruptedIOException::InterruptedIOException(const char* message)
    : JxxSuper(message)
{
}

InterruptedIOException::InterruptedIOException(const std::string& message)
    : JxxSuper(message)
{
}

jxx::Ptr<jxx::lang::Object> InterruptedIOException::cloneImpl() const
{
    return jxx::CAST<jxx::lang::Object>(jxx::NEW<InterruptedIOException>(*this));
}

const char* InterruptedIOException::typeName() const noexcept
{
    return "InterruptedIOException";
}

} // namespace jxx::io
