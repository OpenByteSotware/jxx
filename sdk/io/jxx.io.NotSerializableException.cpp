#include "io/jxx.io.NotSerializableException.h"

#include "lang/jxx.lang.ClassInfo.h"

namespace jxx::io {

jxx::Ptr<jxx::lang::ClassAny> NotSerializableException::Class()
{
    return JxxClassInfoMarker::Class();
}

NotSerializableException::NotSerializableException()
    : Super()
{
}

NotSerializableException::NotSerializableException(const jxx::Ptr<jxx::lang::String>& message)
    : Super(message)
{
}

NotSerializableException::NotSerializableException(const char* message)
    : Super(message)
{
}

NotSerializableException::NotSerializableException(const std::string& message)
    : Super(message)
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
