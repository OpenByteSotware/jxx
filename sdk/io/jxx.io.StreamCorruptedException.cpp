#include "io/jxx.io.StreamCorruptedException.h"

#include "lang/jxx.lang.ClassInfo.h"

namespace jxx::io {

jxx::Ptr<jxx::lang::ClassAny> StreamCorruptedException::Class()
{
    return JxxClassInfoMarker::Class();
}

StreamCorruptedException::StreamCorruptedException()
    : JxxSuper()
{
}

StreamCorruptedException::StreamCorruptedException(const jxx::Ptr<jxx::lang::String>& message)
    : JxxSuper(message)
{
}

StreamCorruptedException::StreamCorruptedException(const char* message)
    : JxxSuper(message)
{
}

StreamCorruptedException::StreamCorruptedException(const std::string& message)
    : JxxSuper(message)
{
}

jxx::Ptr<jxx::lang::Object> StreamCorruptedException::cloneImpl() const
{
    return jxx::CAST<jxx::lang::Object>(jxx::NEW<StreamCorruptedException>(*this));
}

const char* StreamCorruptedException::typeName() const noexcept
{
    return "StreamCorruptedException";
}

} // namespace jxx::io
