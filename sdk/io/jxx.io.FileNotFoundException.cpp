#include "io/jxx.io.FileNotFoundException.h"

#include "lang/jxx.lang.ClassInfo.h"

namespace jxx::io {

jxx::Ptr<jxx::lang::ClassAny> FileNotFoundException::Class()
{
    return JxxClassInfoMarker::Class();
}

FileNotFoundException::FileNotFoundException()
    : JxxSuper()
{
}

FileNotFoundException::FileNotFoundException(const jxx::Ptr<jxx::lang::String>& message)
    : JxxSuper(message)
{
}

FileNotFoundException::FileNotFoundException(const char* message)
    : JxxSuper(message)
{
}

FileNotFoundException::FileNotFoundException(const std::string& message)
    : JxxSuper(message)
{
}

jxx::Ptr<jxx::lang::Object> FileNotFoundException::cloneImpl() const
{
    return jxx::CAST<jxx::lang::Object>(jxx::NEW<FileNotFoundException>(*this));
}

const char* FileNotFoundException::typeName() const noexcept
{
    return "FileNotFoundException";
}

} // namespace jxx::io
