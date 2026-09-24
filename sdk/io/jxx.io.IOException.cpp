#include "io/jxx.io.IOException.h"

#include "lang/jxx.lang.ClassInfo.h"

namespace jxx::io {

jxx::Ptr<jxx::lang::ClassAny> IOException::Class()
{
    return JxxClassInfoMarker::Class();
}

IOException::IOException()
    : Super()
{
}

IOException::IOException(const jxx::Ptr<jxx::lang::String>& message)
    : Super(message)
{
}

IOException::IOException(const char* message)
    : Super(message)
{
}

IOException::IOException(const std::string& message)
    : Super(message)
{
}

jxx::Ptr<jxx::lang::Object> IOException::cloneImpl() const
{
    return jxx::CAST<jxx::lang::Object>(jxx::NEW<IOException>(*this));
}

const char* IOException::typeName() const noexcept
{
    return "IOException";
}

} // namespace jxx::io
