#include "io/jxx.io.NotActiveException.h"

#include "lang/jxx.lang.ClassInfo.h"

namespace jxx::io {

jxx::Ptr<jxx::lang::ClassAny> NotActiveException::Class()
{
    return JxxClassInfoMarker::Class();
}

NotActiveException::NotActiveException()
    : Super()
{
}

NotActiveException::NotActiveException(const jxx::Ptr<jxx::lang::String>& message)
    : Super(message)
{
}

NotActiveException::NotActiveException(const char* message)
    : Super(message)
{
}

NotActiveException::NotActiveException(const std::string& message)
    : Super(message)
{
}

jxx::Ptr<jxx::lang::Object> NotActiveException::cloneImpl() const
{
    return jxx::CAST<jxx::lang::Object>(jxx::NEW<NotActiveException>(*this));
}

const char* NotActiveException::typeName() const noexcept
{
    return "NotActiveException";
}

} // namespace jxx::io
