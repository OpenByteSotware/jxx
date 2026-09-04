#include "io/jxx.io.SyncFailedException.h"

#include "lang/jxx.lang.ClassInfo.h"

namespace jxx::io {

jxx::Ptr<jxx::lang::ClassAny> SyncFailedException::Class()
{
    return JxxClassInfoMarker::Class();
}

SyncFailedException::SyncFailedException()
    : JxxSuper()
{
}

SyncFailedException::SyncFailedException(const jxx::Ptr<jxx::lang::String>& message)
    : JxxSuper(message)
{
}

SyncFailedException::SyncFailedException(const char* message)
    : JxxSuper(message)
{
}

SyncFailedException::SyncFailedException(const std::string& message)
    : JxxSuper(message)
{
}

jxx::Ptr<jxx::lang::Object> SyncFailedException::cloneImpl() const
{
    return jxx::CAST<jxx::lang::Object>(jxx::NEW<SyncFailedException>(*this));
}

const char* SyncFailedException::typeName() const noexcept
{
    return "SyncFailedException";
}

} // namespace jxx::io
