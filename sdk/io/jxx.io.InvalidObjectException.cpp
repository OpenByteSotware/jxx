#include "io/jxx.io.InvalidObjectException.h"

#include "lang/jxx.lang.ClassInfo.h"

namespace jxx::io {

jxx::Ptr<jxx::lang::ClassAny> InvalidObjectException::Class()
{
    return JxxClassInfoMarker::Class();
}

InvalidObjectException::InvalidObjectException()
    : JxxSuper()
{
}

InvalidObjectException::InvalidObjectException(const jxx::Ptr<jxx::lang::String>& message)
    : JxxSuper(message)
{
}

InvalidObjectException::InvalidObjectException(const char* message)
    : JxxSuper(message)
{
}

InvalidObjectException::InvalidObjectException(const std::string& message)
    : JxxSuper(message)
{
}

jxx::Ptr<jxx::lang::Object> InvalidObjectException::cloneImpl() const
{
    return jxx::CAST<jxx::lang::Object>(jxx::NEW<InvalidObjectException>(*this));
}

const char* InvalidObjectException::typeName() const noexcept
{
    return "InvalidObjectException";
}

} // namespace jxx::io
