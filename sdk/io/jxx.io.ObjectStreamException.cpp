#include "io/jxx.io.ObjectStreamException.h"

#include "lang/jxx.lang.ClassInfo.h"

namespace jxx::io {

jxx::Ptr<jxx::lang::ClassAny> ObjectStreamException::Class()
{
    return JxxClassInfoMarker::Class();
}

ObjectStreamException::ObjectStreamException()
    : Super()
{
}

ObjectStreamException::ObjectStreamException(const jxx::Ptr<jxx::lang::String>& message)
    : Super(message)
{
}

ObjectStreamException::ObjectStreamException(const char* message)
    : Super(message)
{
}

ObjectStreamException::ObjectStreamException(const std::string& message)
    : Super(message)
{
}

jxx::Ptr<jxx::lang::Object> ObjectStreamException::cloneImpl() const
{
    return jxx::CAST<jxx::lang::Object>(jxx::NEW<ObjectStreamException>(*this));
}

const char* ObjectStreamException::typeName() const noexcept
{
    return "ObjectStreamException";
}

} // namespace jxx::io
