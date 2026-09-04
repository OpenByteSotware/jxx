#include "io/jxx.io.EOFException.h"

#include "lang/jxx.lang.ClassInfo.h"

namespace jxx::io {

jxx::Ptr<jxx::lang::ClassAny> EOFException::Class()
{
    return JxxClassInfoMarker::Class();
}

EOFException::EOFException()
    : JxxSuper()
{
}

EOFException::EOFException(const jxx::Ptr<jxx::lang::String>& message)
    : JxxSuper(message)
{
}

EOFException::EOFException(const char* message)
    : JxxSuper(message)
{
}

EOFException::EOFException(const std::string& message)
    : JxxSuper(message)
{
}

jxx::Ptr<jxx::lang::Object> EOFException::cloneImpl() const
{
    return jxx::CAST<jxx::lang::Object>(jxx::NEW<EOFException>(*this));
}

const char* EOFException::typeName() const noexcept
{
    return "EOFException";
}

} // namespace jxx::io
