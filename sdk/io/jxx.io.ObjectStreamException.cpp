#include "io/jxx.io.ObjectStreamException.h"

#include "lang/jxx.lang.ClassInfo.h"

namespace jxx::io {

jxx::Ptr<jxx::lang::ClassAny> ObjectStreamException::Class()
{
    return JxxClassInfoMarker::Class();
}

ObjectStreamException::ObjectStreamException()
    : JxxSuper()
{
}

ObjectStreamException::ObjectStreamException(const jxx::Ptr<jxx::lang::String>& message)
    : JxxSuper(message)
{
}

ObjectStreamException::ObjectStreamException(const char* message)
    : JxxSuper(message)
{
}

ObjectStreamException::ObjectStreamException(const std::string& message)
    : JxxSuper(message)
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
