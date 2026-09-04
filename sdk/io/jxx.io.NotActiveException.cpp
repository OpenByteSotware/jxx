#include "io/jxx.io.NotActiveException.h"

#include "lang/jxx.lang.ClassInfo.h"

namespace jxx::io {

jxx::Ptr<jxx::lang::ClassAny> NotActiveException::Class()
{
    return JxxClassInfoMarker::Class();
}

NotActiveException::NotActiveException()
    : JxxSuper()
{
}

NotActiveException::NotActiveException(const jxx::Ptr<jxx::lang::String>& message)
    : JxxSuper(message)
{
}

NotActiveException::NotActiveException(const char* message)
    : JxxSuper(message)
{
}

NotActiveException::NotActiveException(const std::string& message)
    : JxxSuper(message)
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
