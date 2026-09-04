#include "io/jxx.io.NotSerializableException.h"

#include "lang/jxx.lang.ClassInfo.h"

namespace jxx::io {

jxx::Ptr<jxx::lang::ClassAny> NotSerializableException::Class()
{
    return JxxClassInfoMarker::Class();
}

NotSerializableException::NotSerializableException()
    : JxxSuper()
{
}

NotSerializableException::NotSerializableException(const jxx::Ptr<jxx::lang::String>& message)
    : JxxSuper(message)
{
}

NotSerializableException::NotSerializableException(const char* message)
    : JxxSuper(message)
{
}

NotSerializableException::NotSerializableException(const std::string& message)
    : JxxSuper(message)
{
}

jxx::Ptr<jxx::lang::Object> NotSerializableException::cloneImpl() const
{
    return jxx::CAST<jxx::lang::Object>(jxx::NEW<NotSerializableException>(*this));
}

const char* NotSerializableException::typeName() const noexcept
{
    return "NotSerializableException";
}

} // namespace jxx::io
